/*
Quotek Strategies SDK 3.0
Copyright 2013-2016 Quotek SAS
http://www.quotek.io
*/

#ifndef BLACKSHOLES_HPP
#define BLACKSHOLES_HPP

#include <string>
#include "option.hpp"
#include "quant.hpp"

namespace quotek {

  namespace quant {
        

    /** blackscholes provides an option pricing modelisation context. 
     *  Please note that this class only support vanilla options pricing by default,
     *  and if you want to price exotics contracts with this class you'll probably have to inherit it.
     */
    class blackscholes {

      public:
        
        /** black-sholes model object constructor.
         *  @param modelized_option Option object to modelize within Black & Sholes context.
         *  @param underlying_price current price of the underlying asset for modelized option.
         *  @param safe_interest_rate interest safe rate to use for option price calculation.
         *  @param implied_volatility implied price volatility of the underlying asset for modelized option.
         */
      	blackscholes(option& modelized_option,
                    float underlying_price,
                    float safe_interest_rate,
                    float implied_volatility);
        
      	/** blackscholes object destructor */
      	~blackscholes();

        /** compute greeks.
         *  @return a greeks_ structure containing beta, gamme, delta..
         */
      	_greeks compute_greeks();

      	_greeks greeks;
      	
        float price_option();

        option& modelized_option;
        float safe_interest_rate;
        float implied_volatility;
        float underlying_price;

      private:

        float d1;
        float d2;

        float compute_d1();
        float compute_d2();

    };



  }
}

#endif
