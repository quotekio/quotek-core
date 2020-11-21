#ifndef DUMMYB_H
#define DUMMYB_H

#include "../broker.hpp"
#include "../brokerio.hpp"
#include <random>
#include <map>
#include <iostream>

/**
 * Dummy Broker, generates random walks for given indices.
 */
class dummybroker: public broker {

public:
  dummybroker();
  int connect();
  int initialize(string, bool, bool, string);
  void setMode(string);
  string getMode();
  int initPushService();
  int requiresIndicesList();
  int setIndicesList(vector<string>);
  vector<bvex> getValues();
  vector<bpex> getPositions();
  string closePos(string, int size);
  bpex openPos(string, string, int ,float ,float);
  vector<brokerError*>* getErrors();

protected:
   vector<bvex> values;
   vector<string> ilist;
   default_random_engine generator;
   std::map<string, void*> distributions;
   std::map<string, string> disttypes;
   uint32_t nticks;   

   float nextval(string); 
   
};


#endif