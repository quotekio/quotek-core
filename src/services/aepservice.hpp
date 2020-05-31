#ifndef AEPSERVICE_H
#define AEPSERVICE_H

#include <n3rv/n3rvcommon.hpp>
#include <n3rv/n3rvservice.hpp>
#include <n3rv/n3rvservicecontroller.hpp>
#include "../lib/qatecfg.h"
#include "../lib/aep.h"
#include "../lib/aep_ws.hpp"

class aepservice: public n3rv::service {
  using n3rv::service::service;

  public:

      n3rv::qhandler* prices;
      n3rv::qhandler* moneyman;

      int initialize(const char* node_name, qateCfg* cfg) {

        this->cfg = cfg;
        //Sets a global identifier for service node.
        this->set_uid(("qate.aep." + std::string(node_name)).c_str());

        
    
      }

      


  protected: 
    qateCfg* cfg;

};



#endif
