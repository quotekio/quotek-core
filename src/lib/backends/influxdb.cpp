#include "backend.h"
#include <stdlib.h>


class influxdb : public backend {
public:

    virtual int connect() {
      cout << "CONNECT" << endl;
      return 0;
    }

    virtual bdata* query(string q) {

      return NULL;
      
    }

    virtual bdata* query(string indice, int tinf, int tsup) {
      return NULL;
    }

    virtual int store(string indice, bdata* data) {
      return 0;
    }

    virtual int store(string indice, brec* record) {
      return 0;
    }


private:

};

// the class factories
extern "C" backend* create() {
    return new influxdb;
}

extern "C" void destroy(backend* p) {
    delete p;
}
