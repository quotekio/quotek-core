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
        

    /** blacksholes provides an option pricing modelisation context. 
     *  Please note that this class only support vanilla options pricing by default,
     *  and if you want to price exotics contracts with this class you'll probably have to inherit it.
     */
    class blacksholes {

      public:
        
        /** black-sholes model object constructor.
         *  @param modelized_option Option object to modelize within Black & Sholes context.
         *  @param safe_interest_rate interest safe rate to use for option price calculation.
         *  @param implied_volatility implied volatility to use for option price calculation. 
         */
      	blacksholes(option& modelized_option,
                    float safe_interest_rate,
                    float implied_volatility);
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

        float d1;
        float d2;

        float compute_d1();
        float compute_d2();

    };



  }
}

#endif
