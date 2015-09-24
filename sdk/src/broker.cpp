/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include "broker.hpp"

namespace quotek {

  broker::broker(strategy* s) {
    this->s = s;
  }

  void broker::buy(std::string asset,
  	               int quantity,
  	               int stop,
  	               int limit) {

    std::ostringstream os;
    os << "openpos:" << asset << ":buy:" << quantity << ":" << stop << ":" << limit ;
  	s->order(os.str());
  }


  void broker::sell(std::string asset,
  	               int quantity,
  	               int stop,
  	               int limit) {

    std::ostringstream os;
    os << "openpos:" << asset << ":sell:" << quantity << ":" << stop << ":" << limit ;
  	s->order(os.str());
  }

  
  void broker::buy_hedged(std::string asset,
                  std::string hedged_asset,
                  int quantity,
                  int stop,
                  int limit) {

    this->buy(asset,quantity,stop,limit);
    this->sell(hedged_asset,quantity,stop,limit);

  }

  void broker::buy_hedged_asym(std::string asset,
                       int quantity,
                       int stop,
                       int limit,
                       std::string hedged_asset,
                       int hedged_quantity,
                       int hedged_stop,
                       int hedged_limit
                       ) {

    this->buy(asset,quantity,stop,limit);
    this->sell(hedged_asset,hedged_quantity,hedged_stop,hedged_limit);

  }

  void broker::smartbuy(std::string asset, int expexted_risk_reward) {

    std::ostringstream os;
    os << "smartpos:" << asset << ":buy:" << expexted_risk_reward;
    s->order(os.str());

  }

  void broker::smartsell(std::string asset, int expexted_risk_reward) {
    std::ostringstream os;
    os << "smartpos:" << asset << ":sell:" << expexted_risk_reward;
    s->order(os.str());
  }


  void broker::close_position(std::string ticket_id) {
    std::ostringstream os;
    os << "closepos:" << ticket_id;
    s->order(os.str());
  }


}