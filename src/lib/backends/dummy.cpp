#include "backend.h"
#include <stdlib.h>

// the class factories
extern "C" backend* create() {
    return new backend;
}

extern "C" void destroy(backend* p) {
    delete p;
}