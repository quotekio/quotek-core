/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#ifndef BROKER_HPP
#define BROKER_HPP

#include <string>
#include "strategy.hpp"

namespace quotek {

/**
 * broker class is meant to embedd all the methods that interract with the broker 
 * configured by the user.
 */

  class broker {

    public:
 
     /**
      * 
      */
     broker(strategy* s);

     /**
      * buy sends a buy order to the broker
      */
     void buy(std::string asset,
              int quantity,
              int stop,
              int limit);
  

  };
}


#endif