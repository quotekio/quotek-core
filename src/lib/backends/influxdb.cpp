#include "backend.h"
#include <stdlib.h>
#include <influxdb/influxdb.h>
#include "../rapidjson/document.h"
#include "../http.hpp"
#include "../utils.h"
#include <sstream>

class influxdb : public backend {
public:

    virtual int init(string params) {
      
      rapidjson::Document d;
      d.Parse<0>(params.c_str());

      host = d["host"].GetString();
      port = d["port"].GetString();
      username = d["username"].GetString();
      password = d["password"].GetString();
      database = d["database"].GetString();

      pre_url = "http://" + 
                 host + ":" + 
                 port + 
                 "/db/" +
                 database + 
                 "/series" + 
                 "?u=" + 
                 username + 
                 "&p=" + 
                 password + 
                 "&time_precision=s";

      hhdl = new http();
      hhdl->add_header("Content: Application/Json");
      hhdl->add_header("Accept: Application/Json");
      return 0;

    }

    virtual int init(std::string ihost, 
                     int iport, 
                     std::string iusername, 
                     std::string ipassword, 
                     std::string idatabase ) {
      
      host = ihost;
      port = int2string(iport);
      username = iusername;
      password = ipassword;
      database = idatabase;

      pre_url = "http://" + 
                 host + ":" + 
                 port + 
                 "/db/" +
                 database + 
                 "/series" + 
                 "?u=" + 
                 username + 
                 "&p=" + 
                 password + 
                 "&time_precision=s";

      hhdl = new http();
      hhdl->add_header("Content: Application/Json");
      hhdl->add_header("Accept: Application/Json");
      return 0;

    }

    virtual int connect() {
      //influx doesn't need to maintain connection
      return 0;
    }

    virtual records* query(string q) {
      return NULL;
    }

    virtual records* query(string indice, int tinf, int tsup) {

      records* recs;
      records_init(recs,10000);
      std::string url = pre_url;

      //construct query;
      std::ostringstream qstream;
      qstream << "select value, spread from " << indice <<
                 " where time >= " << tinf << 
                 " and time <=" << tsup;

      url += "&q=" + hhdl->escape(qstream.str());

      // Perform http request to influxdb backend and get result.
      std::string res = hhdl->get(url);

      // Effectively parse result
      rapidjson::Document d;
      d.Parse<0>(res.c_str());

      // 
      return recs;

    }

    virtual int store(string indice, records* recs) {

      std::ostringstream sdata;
      sdata << "[\n";
      sdata << "\t{ \"name\": \"" << indice << "\",\n";
      sdata << "\t  \"columns\" : [ \"time\", \"value\", \"spread\" ],\n";
      sdata << "\t  \"points\" : [" << records2json(recs) << "]\n"; 
      sdata << "\t}\n";
      sdata << "]";
      
      cout << sdata.str() << endl;

      string outp = hhdl->post(pre_url,sdata.str());

      cout << "OUT:" << outp << endl;

      return 0;
    }

    virtual int store(string indice, record* rec) {

      std::ostringstream sdata;
      sdata << "[{ \"name\": \"" << indice << ",";
      sdata << "\"columns\" : [ \"time\", \"value\", \"spread\" ],";
      sdata << "\"points\" : [" << record2json(rec) << "]}]";
      hhdl->post(pre_url,sdata.str());
      return 0;
    }

private:
  string host;
  string port;
  string username;
  string password;
  string database;
  string pre_url;
  http* hhdl;

  std::string record2json(record* rec)  {

    std::ostringstream jstream;
    jstream << "[" << rec->timestamp << ", " << rec->value << ", " << rec->spread << "]";
    return jstream.str();
  };

  std::string records2json(records* recs) {

    std::ostringstream jstream;    
    for (int i=0;i<recs->size;i++)  {
      jstream << record2json(&(recs->data[i]));
      if (i !=  (recs->size - 1 ) ) jstream << ",";

    }
    return jstream.str();
  };

};

// the class factories
extern "C" backend* create() {
    return new influxdb;
}

extern "C" void destroy(backend* p) {
    delete p;
}
