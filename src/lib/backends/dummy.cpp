#include "backend.h"
#include <stdlib.h>


class dummydb : public backend {
public:

    virtual int connect() {
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
extern "C" dummydb* create() {
    return new dummydb;
}

extern "C" void destroy(dummydb* p) {
    delete p;
}
