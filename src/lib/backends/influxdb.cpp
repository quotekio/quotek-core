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
      return 0;

    }

    virtual int connect() {
      //influx doesn't need to maintain connection
      return 0;
    }

    virtual records* query(string q) {
      records* result = (records*) malloc(sizeof(records));
      http* hhdl = prepare_http_handler();
      std::string url = pre_url;
      std::string outp;
      record r;

      records_init(result,10000);
      url += "&q=" + hhdl->escape(q);

      // Perform http request to influxdb backend and get result.
      outp = hhdl->get(url);
      // Destroy http handler to free memory.
      hhdl->destroy();

      // Effectively parse result
      rapidjson::Document d;
      d.Parse<0>(outp.c_str());

      if (d.IsArray())  {

        for ( int i=0; i< d[0u]["points"].Size(); i++  ) {
           rapidjson::Value& points = d[0u]["points"][i];           
           r.timestamp = points[0u].GetInt();
           r.value = points[3].GetDouble();
           r.spread = points[2].GetDouble();
           records_push(result,r);
        }
      }

      return result;
    }

    virtual records* query(string indice, int tinf, int tsup) {

      records* result = (records*) malloc(sizeof(records));
      http* hhdl = prepare_http_handler();
      std::string url = pre_url;
      records_init(result,10000);
      std::ostringstream qstream;
      std::string outp;
      record r;

      if (tinf >= 0) {
        qstream << "SELECT value, spread FROM " << indice <<
                   " WHERE time > " << tinf << 
                   "s AND time <" << tsup << "s ORDER ASC";
      }

      else  {

        qstream << "SELECT value, spread FROM " << indice <<
                   " WHERE time > now() + " << tinf << 
                   "s AND time < now() + " << tsup << "s ORDER ASC";
      }

    

      url += "&q=" + hhdl->escape(qstream.str());
      // Perform http request to influxdb backend and get result.
      outp = hhdl->get(url);
      // Destroy http handler to free memory.
      hhdl->destroy();
      
      // Effectively parse result
      rapidjson::Document d;
      d.Parse<0>(outp.c_str());

      if (d.HasParseError()) {
        cout << "[ERROR] Backend query error: JSON Parsing. returning empty result!" << endl;
        return result;
      }

      if (d.IsArray())  {

        for ( int i=0; i< d[0u]["points"].Size(); i++  ) {
           rapidjson::Value& points = d[0u]["points"][i];           
           r.timestamp = points[0u].GetInt();
           r.value = points[3].GetDouble();
           r.spread = points[2].GetDouble();
           records_push(result,r);
        }
      }

      return result;

    }

    virtual int store(string indice, records* recs) {

      std::ostringstream sdata;
      http* hhdl = prepare_http_handler();
      string outp;

      sdata << "[\n";
      sdata << "\t{ \"name\": \"" << indice << "\",\n";
      sdata << "\t  \"columns\" : [ \"time\", \"value\", \"spread\" ],\n";
      sdata << "\t  \"points\" : [" << records2json(recs) << "]\n"; 
      sdata << "\t}\n";
      sdata << "]";

      outp = hhdl->post(pre_url,sdata.str());
      hhdl->destroy();
      return 0;
    }

    virtual int store(string indice, record* rec) {

      std::ostringstream sdata;
      http* hhdl = prepare_http_handler();
      string outp;

      sdata << "[\n";
      sdata << "\t{ \"name\": \"" << indice << "\",\n";
      sdata << "\t  \"columns\" : [ \"time\", \"value\", \"spread\" ],\n";
      sdata << "\t  \"points\" : [" << record2json(rec) << "]\n"; 
      sdata << "\t}\n";
      sdata << "]";

      outp = hhdl->post(pre_url,sdata.str());
      hhdl->destroy();
      return 0;
    }

private:
  string host;
  string port;
  string username;
  string password;
  string database;
  string pre_url;

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

  http* prepare_http_handler() {
    http* hhdl = new http();
    hhdl->add_header("Content: Application/Json");
    hhdl->add_header("Accept: Application/Json");
    return hhdl;
  };


};

// the class factories
extern "C" backend* create() {
    return new influxdb;
}

extern "C" void destroy(backend* p) {
    delete p;
}
