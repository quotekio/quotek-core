/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#ifndef BLACKSHOLES_HPP
#define BLACKSHOLES_HPP

#include <string>
#include "option.hpp"
#include "quant.hpp"

namespace quotek {

  namespace quant {
        
    class blacksholes {

      public:
        
        /** black-sholes model object constructor */
      	blacksholes(option& modelized_option): modelized_option(modelized_option);
      	/** blacksholes object destructor */
      	~blacksholes();

        /** compute greeks.
         *  @return a greeks_ structure containing beta, gamme, delta..
         */
      	_greeks compute_greeks();

      	_greeks greeks;
      	
        float price_option();


      private:

      	option& modelized_option;
        float safe_interest_rate;
        float implied_volatility;

        float compute_d1();
        float compute_d2();

    };



  }
}

#endif
