/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#include "blacksholes.hpp"

namespace quotek {

  namespace quant {
        
      	blacksholes::blacksholes(option& modelized_option,
      		                       float safe_interest_rate,
                                 float implied_volatility): modelized_option(modelized_option) {

      		this->safe_interest_rate = safe_interest_rate;
          this->implied_volatility = implied_volatility;

      	}

      	blacksholes::~blacksholes() {

      	}

      	_greeks blacksholes::compute_greeks() {

          _greeks result;

          //computes greeks according to modelized option kind.
          switch (modelized_option.type) {

          	case option::european_call:
              result.beta = ;
              result.gamma = ;
              result.delta = ;
              result.vega = ;
          	  break;

          	case option::european_put:
              result.beta = ;
              result.gamma = ;
              result.delta = ;
              result.vega = ;
          	  break;

          	case option::us_call:
              result.beta = ;
              result.gamma = ;
              result.delta = ;
              result.vega = ;
          	  break;

          	case option::us_put:
              result.beta = ;
              result.gamma = ;
              result.delta = ;
              result.vega = ;
          	  break;

          	default:
          	  break;

        }
        return result;
      }


     float price_option() {

     }

     float blacksholes::compute_d1() {

     }
     float blacksholes::compute_d2() {

     }
  }
}
