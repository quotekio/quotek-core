#ifndef EVALSERVICE_H
#define EVALSERVICE_H

#include <n3rv/n3rvcommon.hpp>
#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvservicecontroller.hpp>
#include "../lib/qatecfg.h"


class evalservice: public n3rv::service {
  using n3rv::service::service;
  public:

      n3rv::qhandler* prices;
      n3rv::qhandler* moneyman;

      int initialize(const char* node_name, qateCfg* cfg) {

        this->cfg = cfg;
        //Sets a global identifier for service node.
        this->set_uid(("qate.broker." + std::string(node_name)).c_str());


        //Sets a global identifier for service node.
        this->prices = this->connect("qate.broker.*.prices", ZMQ_SUB);  

        this->attach(this->prices, prices_update);

      }

      /*data receive callback.*/
      static void* prices_update(void* objref, zmq::message_t* zmsg) {

      evalservice* self = (evalservice*) objref;
      n3rv::message msg = n3rv::parse_msg(zmsg);
      
      // msg.


      
  }


  protected:

    qateCfg* cfg;



};


/*


  for (int i=0;i<sh_list.size();i++) {

    cout << "preparing strategy compilation for algo " << sh_list[i]->getName() << endl;
    int pres = sh_list[i]->prepareCompile();
    int cerr = 0;

    if (pres == 0) {
      cout << "compiling algo " << sh_list[i]->getName() << endl;
      cerr = sh_list[i]->compile(0);
    }
    else {
      std::cout << "[CRITICAL] Compile prepare failed, shutting down robot" << std::endl;
      exit(1);
    }


    if (cerr > 0) {
      std::cout << "[CRITICAL] Strategy "<< sh_list[i]->getName() << " failed to compile !" << std::endl;
      exit(1);
    }

    cout << "loading compiled algo " << sh_list[i]->getName() << endl;
    sh_list[i]->dlibOpen(0);

  }
  */



#endif