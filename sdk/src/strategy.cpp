/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#include "strategy.hpp"

namespace quotek {

  namespace core {

    strategy::strategy(std::vector<quotek::data::record>& recs,
    	               std::map<std::string, quotek::data::any>& store): recs(recs), store(store) {
     
    }
    
    strategy::~strategy() {

    }

    void strategy::order(std::string order_data) {
      orders_queue.push(order_data);
    }

  }
  
}
