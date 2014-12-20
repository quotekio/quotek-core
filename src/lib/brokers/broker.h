
#ifndef broker_H
#define broker_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;

//Adam broker connector value exchange
typedef struct bvex {
  string epic;
  float buy;
  float sell;
} bvex;

//Adam broker connector position exchange
typedef struct bpex
{
  string dealid;
  string epic;
  int size;
  float open;
  float stop;
  float limit;

} bpex;


class broker {

public:
    broker() {}
    virtual ~broker() {}
    virtual int init(string params) {return 0;}
    virtual int requiresIndicesList() {return 0;}
    virtual int connect() {return 0;}
    virtual int setIndicesList(vector<string> il) { return 0;}
    virtual vector<bvex> getValues() {vector<bvex> v1; return v1; }
    virtual vector<bpex> getPositions() {vector<bpex> p1; return p1; }
    virtual string closePos(string dealid) {return "";};
    virtual string openPos(string epic,string way,int nbc,int stop,int limit) {return "";}

private:

protected:
    vector<string> ilist;
    string username;
    string password;
    string api_key;
    string api_url;
    int requires_indices_list;

};

// the types of the class factories
typedef broker* create_t();
typedef void destroy_t(broker*);

#endif
