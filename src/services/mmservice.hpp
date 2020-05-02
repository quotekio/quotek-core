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
        this->ilist = this->cfg->getIndicesList();
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

      
  void* mmloop() {

   vector<string> si = iGetNames(ilist);
   quotek::data::cvector<quotek::core::position>& poslist = this->mm->getPositions();

   //pnl-needed vars
   float v;
   float cval;
   quotek::data::record r;
   int inc = 0;
   while (1) {

    //checks STOPS & LIMIT and cleans positions if needed.
    for(int i=0;i< poslist.size();i++ ) {

      quotek::core::position& p = poslist[i];

      if ( this->getAssetRecords(p.asset_name).size() == 0 ) continue;
      r = this->getAssetRecords(p.asset_name).last();
      cval = r.value;

      //checking of Real Stops
    
      if ( p.size < 0  &&  cval >= p.stop ) {
        mm->remPosition(p.ticket_id);
        logger->log("Position " +  p.ticket_id + " closed ! (STOP)");
        continue;
      }

      else if ( p.size > 0  &&  cval <= p.stop ) {
        mm->remPosition(p.ticket_id);
        logger->log("Position " +  p.ticket_id + " closed ! (STOP)");
        continue;
      }

      //checking of virtual Stops

      float vstop = p.get_vstop();

      if ( vstop != p.stop ) {

        if (p.size < 0 && cval >= vstop) {
         closePosition(p.ticket_id);
         logger->log("Position " +  p.ticket_id + " closed ! (VSTOP)");
         continue;
        }

        else if (p.size > 0 && cval <= vstop) {
         closePosition(p.ticket_id);
         logger->log("Position " +  p.ticket_id + " closed ! (VSTOP)");
         continue;
        }
      }

      if (p.limit > 0) {

        if ( p.size < 0  &&  cval < p.limit ) {
          mm->remPosition(p.ticket_id);
          logger->log("Position " +  p.ticket_id + " closed ! (LIMIT)");
          continue;
        }

        else if ( p.size > 0  &&  cval > p.limit ) {
          mm->remPosition(p.ticket_id);
          logger->log("Position " +  p.ticket_id + " closed ! (LIMIT)");
          continue;
        }
      }
    }

    for(int j=0;j<si.size();j++) {
      
      quotek::data::records& mrecs = this->getAssetRecords(si.at(j));
        if ( mrecs.size() > 0 ) {
          quotek::data::record& mr = mrecs.last();
          v = mr.value;
          //cout << v << endl;
          mm->computePNLs(si.at(j),v);
      }
    }

    if (inc == 10) {
      //logger->log("Current PNL:" + float2string(mm->computeWholePNL()) );
      float cp = mm->computeWholePNL();
  
      //The shit just hit the fan !
      if ( ! mm->heartbeat() ) {
        
        cerr << "* CRITICAL LOSS, BLOCKING MONEY MANAGER!*" << std::endl;
        logger->log("* CRITICAL LOSS, BLOCKING MONEY MANAGER! *");

      }

      mm->saveCPNL();
      inc = 0;
    }
    
    inc++;

    usleep(1000000);
         } 

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
     AssocArray<indice*> ilist;

};

#endif