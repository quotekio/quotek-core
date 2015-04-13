
#ifndef BACKEND_H
#define BACKEND_H

#include <vector>
#include <string>
#include <iostream>
#include "../records.h"
#include "../position.h"

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

    /**
     * Saves trades history to backend database.
     * @param plist: pointer to vector of positions to save.
     * @returns: 0 on success, >0 either.
     */
    virtual int saveHistory(vector<position>* plist) {return 0;}

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
