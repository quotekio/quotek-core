/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#include "blackscholes.hpp"

namespace quotek {

  namespace quant {
        
      	blackscholes::blackscholes(option& modelized_option,
      		                       float safe_interest_rate,
                                 float implied_volatility): modelized_option(modelized_option) {

      		this->safe_interest_rate = safe_interest_rate;
          this->implied_volatility = implied_volatility;

      	}

      	blackscholes::~blackscholes() {

      	}

      	_greeks blackscholes::compute_greeks() {

          _greeks result;

          //computes greeks according to modelized option kind.
          switch (modelized_option.type) {

          	case option::european_call:
              result.beta = 0;
              result.gamma = 0;
              result.delta = 0;
              result.vega = 0;
          	  break;

          	case option::european_put:
              result.beta = 0;
              result.gamma = 0;
              result.delta = 0;
              result.vega = 0;
          	  break;

          	case option::us_call:
              result.beta = 0;
              result.gamma = 0;
              result.delta = 0;
              result.vega = 0;
          	  break;

          	case option::us_put:
              result.beta = 0;
              result.gamma = 0;
              result.delta = 0;
              result.vega = 0;
          	  break;

          	default:
          	  break;

        }
        return result;
      }


     float blackscholes::price_option() {
       float result;
       return result;
     }
     
     float blackscholes::compute_d1() {
      float result;
      this->d1 = result;
      return result;
     }

     float blackscholes::compute_d2() {
       float result;
       this->d2 = result;
       return result;
     }
  }
}
