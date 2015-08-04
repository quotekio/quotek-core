/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include "option.hpp"

namespace quotek {

  namespace quant {
    
    option::option() {

    }

    option::option(int type,
        	   std::string underlying,
        	   float price,
        	   float strike,
        	   float expires) {

      this->type = type;
      this->underlying = underlying;
      this->price = price;
      this->strike = strike;
      this->expires = expires;
    }

    option::~option() {

    }

      
    bool option::is_in_money() {
      return in_money;
    }
  }
}