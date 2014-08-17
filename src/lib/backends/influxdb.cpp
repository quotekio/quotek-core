#include "backend.h"
#include <stdlib.h>
#include <influxdb/influxdb.h>
#include "../rapidjson/document.h"

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

      return 0;

    }

    virtual int connect() {
      string host_port = host + ":" + port;

      char hp[host_port.length()+1];
      char un[username.length()+1];
      char pw[password.length()+1];
      char db[database.length()+1];

      strcpy(hp,host_port.c_str());
      strcpy(un,username.c_str());
      strcpy(pw,password.c_str());
      strcpy(db,database.c_str());

      client = influxdb_client_new(hp,un,pw,db, 0);

      return 0;
    }

    virtual records* query(string q) {
      return NULL;
    }

    virtual records* query(string indice, int tinf, int tsup) {
      return NULL;
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

};

// the class factories
extern "C" backend* create() {
    return new influxdb;
}

extern "C" void destroy(backend* p) {
    delete p;
}
