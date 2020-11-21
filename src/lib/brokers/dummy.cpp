#include "dummy.hpp"

dummybroker::dummybroker() {

}

int dummybroker::connect() {
    return 0;
}

int dummybroker::initialize(string, bool, bool, string) {

     this->nticks = 1000000;
     return 0;
}

void dummybroker::setMode(string) {
     return;
}

string dummybroker::getMode() {
    return this->connector_mode;
}

int dummybroker::initPushService() {

}

int dummybroker::requiresIndicesList() {
  return 1;
}

int dummybroker::setIndicesList(vector<string> il) {
  this->ilist = il;

  for (auto idx: this->ilist) {
      this->distributions[idx] =  (void*) new std::binomial_distribution<int>(500 * 10,0.5);
      this->disttypes[idx] = "binom";
  }

  return 0;
}



float dummybroker::nextval(string idx) {

    float n = 0;

    if ( this->disttypes[idx] == "binom" ) {
       
        std::binomial_distribution<float>* dist = static_cast<std::binomial_distribution<float>*>(this->distributions[idx]);
        n = (float) dist->operator() / 10;
    }

    return n;

}

vector<bvex> dummybroker::getValues() {

    this->values.clear();
    for (auto idx: this->ilist) {

         float n = this->nextval(idx);
         bvex e;
         e.epic = idx;
         e.bid = n-1;
         e.offer = n+1;
         this->values.push_back(e);
    }    

    return this->values;
}

vector<bpex> dummybroker::getPositions() {
    return vector<bpex>();
}

string dummybroker::closePos(string, int size) {
    return "";
}

bpex dummybroker::openPos(string, string, int ,float ,float) {
    return bpex();
}

vector<brokerError*>* dummybroker::getErrors() {
  return nullptr;
}

// the class factories
extern "C" broker* create() {
    return new dummybroker();
}

extern "C" void destroy(broker* p) {
    delete p;
}