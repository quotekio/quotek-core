#ifndef BROKERSERVICE_H
#define BROKERSERVICE_H

#include <n3rv/n3rvcommon.hpp>
#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvservicecontroller.hpp>
#include "../lib/qatecfg.h"
#include "../lib/broker.hpp"
#include <dlfcn.h>


class brokerservice: public n3rv::service {
  using n3rv::service::service;
  public:

      n3rv::qhandler* prices;
      n3rv::qhandler* orders;

      int initialize(const char* node_name, qateCfg* cfg) {

        this->cfg = cfg;
        //Sets a global identifier for service node.
        this->set_uid(("qate.broker." + std::string(node_name)).c_str());

        //instanciates broker object
        this->broker = this->load_broker(this->cfg->getBroker())();

        this->prices = this->bind("prices", "127.0.0.1", ZMQ_PUB);  
        this->orders = this->bind("orders","127.0.0.1", ZMQ_REP);
        this->attach(this->orders, process_orders);

      }

      void hkloop() {
        
        //Publishes fetched prices to stream.

      }
      
      static void* process_orders(void* objref, zmq::message_t* zmsg) {

        brokerservice* self = (brokerservice*) objref;
        n3rv::message msg = n3rv::parse_msg(zmsg);
      
        // msg.
      }

  protected:

     qateCfg* cfg;
     broker* broker;

     create_t* load_broker(string bname)  {
       std::cout << "loading broker module..." << endl;
       string lib_broker = "lib" + bname + ".so";
       void* handle = dlopen(lib_broker.c_str(),RTLD_LAZY);

       if(handle == NULL){
         std::cerr << dlerror() << endl;
         exit(1);
       }

       create_t* create_broker = (create_t*) dlsym(handle, "create");

       const char* dlsym_error = dlerror();
         if (dlsym_error) {
          std::cerr << "Cannot load symbol create: " << dlsym_error << endl;
          exit(1);
        }
       return create_broker;
    }

};

#endif