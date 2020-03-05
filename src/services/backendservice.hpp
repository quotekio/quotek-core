#ifndef BACKENDSERVICE_H
#define BACKENDSERVICE_H

#include <string>
#include <cstdio>
#include <cstdlib>

#include <n3rv/n3rvcommon.hpp>
#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvservicecontroller.hpp>
#include <quotek/backend.hpp>
#include "../lib/qatecfg.h"
#include <dlfcn.h>


class backendservice: public n3rv::service {
  using n3rv::service::service;
  public:

      n3rv::qhandler* prices;
      n3rv::qhandler* data;

      int initialize(const char* node_name, qateCfg* cfg) {

        this->cfg = cfg;

        this-back = this->load_backend(c->getBackend());


        //Sets a global identifier for service node.
        this->set_uid(("qate.backend." + std::string(node_name)).c_str());

        //instanciates broker object
        //this->broker = this->load_backend(this->cfg->getBroker())();
        this->prices = this->connect("qate.broker.*.prices",ZMQ_SUB);
        this->data = this->bind("data","127.0.0.1", ZMQ_REP);
        //this->attach(this->orders, process_orders);

      }
 
      /** Distributes data to requester service */
      static void* dist_data(void* objref, zmq::message_t* zmsg) {
        backendservice* self = (backendservice*) objref;
        n3rv::message msg = n3rv::parse_msg(zmsg);      
      }

      /** Receives prices coming from broker and stores'em in database. */
      static void* prices_receive(void* objref, zmq::message_t* zmsg) {

      }


  protected:

    backend* back;
    qateCfg* cfg;

    create_be* load_backend(std::string bename) {   

      std::cout << "loading backend module..." << std::endl;
      std::string lib_backend = "lib" + bename + ".so";
      void* handle = dlopen(lib_backend.c_str(),RTLD_LAZY);

      if(handle == NULL){
        std::cerr << dlerror() << std::endl;
        exit(1);
      }

      create_be* create_backend = (create_be*) dlsym(handle, "create");
      const char* dlsym_error = dlerror();
      if (dlsym_error) {
          std::cerr << "Cannot load symbol create: " << dlsym_error << std::endl;
          exit(1);
      }
      return create_backend;
    }

};

#endif
