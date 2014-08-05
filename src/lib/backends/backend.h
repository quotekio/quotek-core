
#ifndef BACKEND_H
#define BACKEND_H

#include <vector>
#include <string>
#include <iostream>
#include "../narrays.h"

using namespace std;


typedef struct __brec {

  float value;
  float spread;

} brec;

typedef struct __bdata {

  iarray* timestamps;
  brec* records; 

} bdata;


class backend {

public:
    backend() {}
    virtual ~backend() {}
    virtual int connect() {return 0;}
    virtual bdata* query(string) {return NULL;}
    virtual bdata* query(string, int, int) {return NULL;}
    virtual int store(string,bdata*) {return 0;}
    virtual int store(string,brec*) {return 0;}

};

// the types of the class factories
typedef backend* create_t();
typedef void destroy_t(backend*);

#endif
