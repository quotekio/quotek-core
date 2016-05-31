
#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <string>
#include <iostream>
#include <quotek/record.hpp>

using namespace std;

typedef struct cacheio {
 
  int* timestamps;
  float* values;
  float* spreads;
  size_t size;

} cacheio;


class cache {

public:
    cache() {}
    virtual ~cache() {}
    virtual int init(string host, int port) {return 0;}
    virtual int connect() {return 0;}
    virtual quotek::data::records query(string indice, 
                                        int tinf, 
                                        int tsup) {quotek::data::records v1;
                                                   return v1; }
                                           
    virtual int store(string indice , 
                      int tinf, 
                      int tsup, 
                      quotek::data::records& recs) {return 0;}


protected:

  void to_cache(quotek::data::records& recs, cacheio* cio) {

    cio->size = recs.size();
    cio->values = (float*) malloc ( cio->size * sizeof(float) + 1  );
    cio->timestamps = (int*) malloc ( cio->size * sizeof(int) + 1  );
    cio->spreads = (float*) malloc ( cio->size * sizeof(float) + 1  );

    for (int i=0; i < recs.size();i++ ) {

      cio->values[i] = recs[i].value;
      cio->timestamps[i] = recs[i].timestamp;
      cio->spreads[i] = recs[i].spread;

    }

  }

  void from_cache(quotek::data::records& recs, cacheio* cio) {

    
    for (int i=0;i< cio->size; i++) {
      recs.append(cio->timestamps[i], cio->values[i], cio->spreads[i] );
    }
  }

};


// the types of the class factories
typedef cache* create_cc();
typedef void destroy_cc(cache*);

#endif
