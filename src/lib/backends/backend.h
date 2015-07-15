
#ifndef BACKEND_H
#define BACKEND_H

#include <vector>
#include <string>
#include <iostream>
#include "../position.h"

#include <quotek/cvector.hpp>
#include <quotek/record.hpp>

using namespace std;

class backend {

public:
    backend() {}
    virtual ~backend() {}
    virtual int init(string) {return 0;}
    virtual int connect() {return 0;}
    virtual std::vector<quotek::data::record> query(string) {return NULL;}
    virtual std::vector<quotek::data::record> query(string, int, int) {return NULL;}
    virtual int store(string,quotek::data::record&) {return 0;}
    virtual int store(string,std::vector<quotek::data::record>&) {return 0;}

    /**
     * Saves trades history to backend database.
     * @param plist: pointer to vector of positions to save.
     * @returns: 0 on success, >0 either.
     */
    virtual int saveHistory(quotek::data::cvector<position>* plist) {return 0;}

    /**
     * Saves a position history to backend database.
     * @param pos: pointer to position object to save.
     * @returns: 0 on success, >0 either.
     */
    virtual int saveHistory(position* pos) {return 0;}

};

// the types of the class factories
typedef backend* create_be();
typedef void destroy_be(backend*);

#endif
