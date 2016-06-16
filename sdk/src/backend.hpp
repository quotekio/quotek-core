/*
Quotek Strategies SDK 3.0
Copyright 2013-2016 Quotek SAS
http://www.quotek.io
*/

#ifndef BACKEND_HPP
#define BACKEND_HPP

#include <vector>
#include <string>
#include <iostream>

#include "cvector.hpp"
#include "position.hpp"
#include "record.hpp"

using namespace std;


class backend {

public:
    backend() {}
    virtual ~backend() {}
    virtual int init(string) {return 0;}
    virtual int connect() {return 0;}
    virtual quotek::data::records query(string) { quotek::data::records v1;
                                                              return v1; }
    virtual quotek::data::records query(string, int, int) {quotek::data::records v1;
                                                                       return v1; }
                                                                       
    virtual int store(string,quotek::data::record&) {return 0;}
    virtual int store(string,std::vector<quotek::data::record>&) {return 0;}

    /**
     * save() is a function meant to store data coming from quotek algo variables.
     * @param long timestamp if != 0, the record is timestamped with timestamp instead of current time.
     * @param tag tagged name of the value, to find it back later.
     * @param data data to store, as a string.
     * @return 0 on succes, > 0 either.
     */
    virtual int save(long timestamp, 
                     std::string tag, 
                     std::string data) { return 0;}

    /**
     * Saves trades history to backend database.
     * @param plist: pointer to vector of positions to save.
     * @returns: 0 on success, >0 either.
     */
    virtual int saveHistory(quotek::data::cvector<quotek::core::position>& plist) {return 0;}

    /**
     * Saves a position history to backend database.
     * @param pos: pointer to position object to save.
     * @returns: 0 on success, >0 either.
     */
    virtual int saveHistory(quotek::core::position& pos) {return 0;}

};

// the types of the class factories
typedef backend* create_be();
typedef void destroy_be(backend*);

#endif