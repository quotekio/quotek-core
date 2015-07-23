/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#include "option.hpp"

namespace quotek {

  namespace quant {
    
    int option::call = 0x01;
    int option::put = 0x02;

   
    option::option() {

    }

    option::option(int type,
        	   std::string underlying,
        	   float price,
        	   float strike,
        	   long expires) {

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