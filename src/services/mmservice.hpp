#ifndef MMSERVICE_H
#define MMSERVICE_H

#include <n3rv/n3rvcommon.hpp>
#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvservicecontroller.hpp>
#include "../lib/qatecfg.h"
#include "../lib/moneymanager.h"


class mmservice: public n3rv::service {
  using n3rv::service::service;
  public:

      n3rv::qhandler* prices;
      n3rv::qhandler* broker_orders;
      n3rv::qhandler* orders;
      n3rv::qhandler* broker;

      int initialize(const char* node_name, qateCfg* cfg) {

        this->cfg = cfg;

        //Initializes moneymanager
        mm_params* mmp = this->cfg->getMMP();
        AssocArray<indice*> ilist = this->cfg->getIndicesList();
        igmLogger* logger = new igmLogger();

        this->mm = new moneyManager(mmp->mm_capital,
                                      mmp->mm_max_openpos, 
                                      mmp->mm_max_openpos_per_epic, 
                                      mmp->mm_reverse_pos_lock, 
                                      mmp->mm_reverse_pos_force_close, 
                                      mmp->mm_max_loss_percentage_per_trade,
                                      mmp->mm_critical_loss_percentage, 
                                      mmp->mm_max_var,
                                      ilist,
                                      logger);


        //Sets a global identifier for service node.
        this->set_uid(("qate.broker." + std::string(node_name)).c_str());

        this->prices = this->connect("qate.broker.*.prices", ZMQ_SUB);
        this->broker_orders = this->connect("qate.broker.*.orders", ZMQ_REQ);
        this->orders = this->bind("orders","0.0.0.0",ZMQ_REP);

        this->attach(this->orders, process_orders);
        this->attach(this->prices, update_pnl);
        

      }

      void hkloop() {
        
        //Publishes fetched prices to stream.
      }
      
      static void* process_orders(void* objref, zmq::message_t* zmsg) {

        mmservice* self = (mmservice*) objref;
        n3rv::message msg = n3rv::parse_msg(zmsg);
      
        // msg.
      }

      static void* update_pnl(void* objref, zmq::message_t* zmsg) {

          mmservice* self = (mmservice*) objref;
          n3rv::message msg = n3rv::parse_msg(zmsg);
        
      }



  protected:

     qateCfg* cfg;
     moneyManager* mm;

};

#endif