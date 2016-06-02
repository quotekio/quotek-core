#include "cache.h"
#include "../utils.h"
#include <sstream>
#include <hiredis/hiredis.h>

#include <iomanip>

class redis : public cache {
public:

    virtual int init(std::string host, 
                     int port) {
      
      this->host = host;
      this->port = port;

      return 0;

    }

    virtual int connect() {
      
      struct timeval timeout = { 1, 500000 }; // 1.5 seconds

      rdx = redisConnectWithTimeout(host.c_str(), port, timeout);
      if (rdx == NULL || rdx->err) {
        if (rdx) {
          printf("Connection error: %s\n", rdx->errstr);
          redisFree(rdx);
        } 

        else {
            printf("Connection error: can't allocate redis context\n");
        }
        exit(1);
      }

    }

    virtual quotek::data::records query(string indice, int tinf, int tsup) {

      quotek::data::records result;
      cacheio cio;

      std::stringstream key_root;
      key_root <<  indice << "_" << tinf << "_" << tsup ;

      std::string key_size = key_root.str() + "_size" ;
      std::string key_values = key_root.str() + "_values" ;
      std::string key_timestamps = key_root.str() + "_timestamps" ;
      std::string key_spreads = key_root.str() + "_spreads" ;

      redisReply *reply;
      
      reply = (redisReply *) redisCommand(rdx, "SELECT 9");

      reply = (redisReply *) redisCommand(rdx, 
                                          "GET %s", 
                                           key_size.c_str()
                                          );

       

      if ( reply->type != REDIS_REPLY_STRING ) return result;


      int f;
      memcpy(&f,reply->str, reply->len);

      cio.size = (size_t) f;

      //debug
      //std::cout << "SIZE OK" << std::endl;

      cio.values = (float*) malloc ( cio.size * sizeof(float) + 1  );
      cio.timestamps = (int*) malloc ( cio.size * sizeof(long) + 1  );
      cio.spreads = (float*) malloc ( cio.size * sizeof(float) + 1  );

      reply = (redisReply *) redisCommand(rdx, 
                                          "GET %s", 
                                           key_values.c_str()
                                          );

      
      if ( reply->type != REDIS_REPLY_STRING ) return result;

      memcpy(cio.values,reply->str, reply->len);
      freeReplyObject(reply);
 
      //debug
      //std::cout << "VALUES OK" << std::endl;

      reply = (redisReply *) redisCommand(rdx, 
                                          "GET %s", 
                                           key_timestamps.c_str()
                                          );

      if ( reply->type != REDIS_REPLY_STRING ) return result;

      std::cout << "TSTAMPS REPL OK" << std::endl;

      memcpy(cio.timestamps,reply->str, reply->len);
      freeReplyObject(reply);

      //debug
      //std::cout << "TSTAMPS OK" << std::endl;

      reply = (redisReply *) redisCommand(rdx, 
                                          "GET %s", 
                                           key_spreads.c_str()
                                          );

      if ( reply->type != REDIS_REPLY_STRING ) return result;

      memcpy(cio.spreads,reply->str, reply->len);
      freeReplyObject(reply);

      //debug
      //std::cout << "SPREADS OK" << std::endl;

      from_cache(result,&cio);
      
      return result;

    }

    virtual int store(string indice, int tinf, int tsup, quotek::data::records& recs) {

      std::cout << "Storing Data to Cache" << std::endl;

      std::stringstream key_root;
      key_root <<  indice << "_" << tinf << "_" << tsup ;
      
      std::string key_size = key_root.str() + "_size" ;
      std::string key_values = key_root.str() + "_values" ;
      std::string key_timestamps = key_root.str() + "_timestamps" ;
      std::string key_spreads = key_root.str() + "_spreads" ;

      cacheio cio;

      to_cache(recs,&cio);
      
      if (rdx == NULL || rdx->err) this->connect();

      int* x = (int*) malloc(sizeof(int));
      *x = cio.size;

      redisReply *reply;
      
      reply = (redisReply *) redisCommand(rdx, "SELECT 9");

      reply = (redisReply *) redisCommand(rdx, 
                                          "SET %s %b", 
                                           key_size.c_str(),
                                           x,
                                           sizeof(int));
      
                         
      reply = (redisReply *) redisCommand(rdx, 
                                          "SET %s %b", 
                                           key_values.c_str(), 
                                           cio.values, 
                                           sizeof(float) * cio.size );

      reply = (redisReply *) redisCommand(rdx, 
                                          "SET %s %b", 
                                           key_timestamps.c_str(),
                                           cio.timestamps, 
                                           sizeof(long) * cio.size );

      
      reply = (redisReply *) redisCommand(rdx, 
                                          "SET %s %b", 
                                           key_spreads.c_str(), 
                                           cio.spreads, 
                                           sizeof(float) * cio.size ); 
    
      free(cio.values);
      free(cio.timestamps);
      free(cio.spreads);

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
