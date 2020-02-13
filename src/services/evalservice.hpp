#ifndef EVALSERVICE_H
#define EVALSERVICE_H

#include <n3rv/n3rvcommon.hpp>
#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvservicecontroller.hpp>

class evalservice: public n3rv::service {
  using n3rv::service::service;
  public:

      n3rv::qhandler* prices;

      int initialize() {

        //Sets a global identifier for service node.
        this->set_uid("qate.eval.eval1");
        this->prices = this->connect("qate.broker.*.prices", ZMQ_SUB);  

        this->attach(this->prices, prices_update);

      }

      /*data receive callback.*/
      static void* prices_update(void* objref, zmq::message_t* zmsg) {

      evalservice* self = (evalservice*) objref;
      n3rv::message msg = n3rv::parse_msg(zmsg);
      
      // msg.


      
  }



};

#endif