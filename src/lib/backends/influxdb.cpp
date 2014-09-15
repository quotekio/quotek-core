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
      return 0;
    }

    virtual int store(string indice, record* rec) {
      return 0;
    }

private:
  s_influxdb_client* client;
  string host;
  string port;
  string username;
  string password;
  string database;
  string pre_url;

  http* hhdl;

};

// the class factories
extern "C" backend* create() {
    return new influxdb;
}

extern "C" void destroy(backend* p) {
    delete p;
}
