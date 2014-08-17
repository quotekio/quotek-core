
#ifndef BACKEND_H
#define BACKEND_H

#include <vector>
#include <string>
#include <iostream>
#include "../records.h"

using namespace std;

class backend {

public:
    backend() {}
    virtual ~backend() {}
    virtual int init(string) {return 0;}
    virtual int connect() {return 0;}
    virtual records* query(string) {return NULL;}
    virtual records* query(string, int, int) {return NULL;}
    virtual int store(string,records*) {return 0;}
    virtual int store(string,record*) {return 0;}

};

// the types of the class factories
typedef backend* create_be();
typedef void destroy_be(backend*);

#endif
