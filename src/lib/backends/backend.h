
#ifndef BACKEND_H
#define BACKEND_H

#include <vector>
#include <string>
#include <iostream>

#include <quotek/cvector.hpp>
#include <quotek/position.hpp>
#include <quotek/record.hpp>

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
     * Saves trades history to backend database.
     * @param plist: pointer to vector of positions to save.
     * @returns: 0 on success, >0 either.
     */
    virtual int saveHistory(quotek::data::cvector<quotek::core::position>* plist) {return 0;}

    /**
     * Saves a position history to backend database.
     * @param pos: pointer to position object to save.
     * @returns: 0 on success, >0 either.
     */
    virtual int saveHistory(quotek::core::position* pos) {return 0;}

};

// the types of the class factories
typedef backend* create_be();
typedef void destroy_be(backend*);

#endif
