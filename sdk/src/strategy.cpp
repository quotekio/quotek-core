/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#include "strategy.hpp"

namespace quotek {

  namespace core {

    strategy::strategy(quotek::data::records& recs,
    	               std::map<std::string, quotek::data::any>& store,
                     std::vector<quotek::core::position>& portfolio
                     ): recs(recs), store(store), portfolio(portfolio)  {
     
    }

    strategy::~strategy() {

    }

    void strategy::order(std::string order_data) {
      orders_queue.push(order_data);
    }

    void strategy::log(std::string log_string) {
      log_queue.push(log_string);
    }

  }
}
