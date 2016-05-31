#include "cache.h"
#include "../utils.h"
#include <sstream>
#include <hiredis/hiredis.h>

class redis : public cache {
public:

    virtual int init(std::string host, 
                     int port) {
      
      this->host = host;
      this->port = port;

      return 0;

    }

    virtual int connect() {
      
      rdx = redisConnect(host.c_str(),port);
      return 0;
    }

    virtual quotek::data::records query(string indice, int tinf, int tsup) {

      quotek::data::records result;

      std::stringstream key_root;
      key_root <<  indice << "_" << tinf << "_" << tsup ;

      std::string key_size = key_root.str() + "_size" ;
      std::string key_values = key_root.str() + "_values" ;
      std::string key_timestamps = key_root.str() + "_timestamps" ;
      std::string key_spreads = key_root.str() + "_spreads" ;

      return result;

    }

    virtual int store(string indice, int tinf, int tsup, quotek::data::records& recs) {

      std::stringstream key_root;
      key_root <<  indice << "_" << tinf << "_" << tsup ;
      
      std::string key_size = key_root.str() + "_size" ;
      std::string key_values = key_root.str() + "_values" ;
      std::string key_timestamps = key_root.str() + "_timestamps" ;
      std::string key_spreads = key_root.str() + "_spreads" ;

      cacheio cio;

      to_cache(recs,&cio);
      
      redisReply *reply;

      reply = (redisReply *) redisCommand(rdx, 
                                          "SET %b %b", 
                                           key_size.c_str(), 
                                           (size_t) sizeof(key_size), 
                                           cio.size, 
                                           (size_t) sizeof(int) );

      reply = (redisReply *) redisCommand(rdx, 
                                          "SET %b %b", 
                                           key_values.c_str(), 
                                           (size_t) sizeof(key_values), 
                                           *cio.values, 
                                           (size_t) sizeof(*cio.values) );

      reply = (redisReply *) redisCommand(rdx, 
                                          "SET %b %b", 
                                           key_timestamps.c_str(), 
                                           (size_t) sizeof(key_timestamps), 
                                           *cio.timestamps, 
                                           (size_t) sizeof(*cio.timestamps) );

      reply = (redisReply *) redisCommand(rdx, 
                                          "SET %b %b", 
                                           key_spreads.c_str(), 
                                           (size_t) sizeof(key_spreads), 
                                           *cio.spreads, 
                                           (size_t) sizeof(*cio.spreads) );      


      if (!reply) return REDIS_ERR;
      freeReplyObject(reply);

      return 0;
      
    }

private:
  string host;
  int port;
  redisContext *rdx;

  
};

// the class factories
extern "C" cache* create() {
    return new redis;
}

extern "C" void destroy(cache* p) {
    delete p;
}
