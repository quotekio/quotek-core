#include "adam/tsemodule.hpp"

class performance: public tsemodule  {


  int init()  {

  }

  int run() {

  }

};

/* Class mangling */
extern "C" tsemodule* create(tsexport* tse) {
    return new performance(tse);
}

extern "C" void destroy(tsemodule* tsemod) {
    delete tsemod;
}





