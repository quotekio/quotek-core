
#ifndef broker_H
#define broker_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;


class broker {

public:
    broker() {}
    virtual ~broker() {}
    virtual int connect() {return 0;}
    virtual string getValues() {return "";}
    virtual string getPositions() {return "";}
    virtual string closePos(string dealid) {return "";};
    virtual string openPos(string epic,string way,int nbc,int stop,int limit) {return "";}

};

// the types of the class factories
typedef broker* create_t();
typedef void destroy_t(broker*);

#endif
