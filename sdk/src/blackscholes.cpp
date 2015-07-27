/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#include "blackscholes.hpp"

namespace quotek {

  namespace quant {
        
      	blackscholes::blackscholes(option& modelized_option,
                                   float underlying_price,
      		                         float safe_interest_rate,
                                   float implied_volatility): modelized_option(modelized_option) {

          this->underlying_price = underlying_price;
      		this->safe_interest_rate = safe_interest_rate;
          this->implied_volatility = implied_volatility;

      	}

      	blackscholes::~blackscholes() {

      	}

      	_greeks blackscholes::compute_greeks() {

          this->compute_d1();
          this->compute_d2();

          //computes greeks according to modelized option kind.
          switch (modelized_option.type) {

          	case option::european_call:
              greeks.delta = exp(-1 * safe_interest_rate * modelized_option.expires) * quotek::quant::N(this->d1);
              greeks.gamma = 0;
              greeks.theta = 0;
              greeks.rho = 1/100 * (modelized_option.strike *  modelized_option.expires * 
                            exp(-1 * safe_interest_rate * modelized_option.expires ) )  * quotek::quant::N(this->d2) ;
              greeks.vega = 0;
          	  break;

          	case option::european_put:
              greeks.delta = exp(-1 * safe_interest_rate * modelized_option.expires) * (quotek::quant::N(this->d1)-1);
              greeks.gamma = 0;
              greeks.theta = 0;
              greeks.rho = -1/100 * (modelized_option.strike *  modelized_option.expires * 
                            exp( -1 * safe_interest_rate * modelized_option.expires ) )  * quotek::quant::N(-1 * this->d2) ;
              greeks.vega = 0;
          	  break;

            case option::us_call:
              greeks.delta = exp(-1 * safe_interest_rate * modelized_option.expires) * quotek::quant::N(this->d1);
              greeks.gamma = 0;
              greeks.theta = 0;
              greeks.rho = 1/100 * (modelized_option.strike *  modelized_option.expires * 
                            exp(-1 * safe_interest_rate * modelized_option.expires ) )  * quotek::quant::N(this->d2) ;
              greeks.vega = 0;
              break;

            case option::us_put:
              greeks.delta = exp(-1* safe_interest_rate * modelized_option.expires) * (quotek::quant::N(this->d1)-1);
              greeks.gamma = 0;
              greeks.theta = 0;
              greeks.rho = -1/100 * (modelized_option.strike *  modelized_option.expires * 
                            exp(-1 * safe_interest_rate * modelized_option.expires ) )  * quotek::quant::N(-1 * this->d2) ;
              greeks.vega = 0;
              break;
          
          	default:
          	  break;

        }
        return greeks;
      }


     float blackscholes::price_option() {
       float result;
       
       compute_d1();
       compute_d2();

       switch (modelized_option.type) {

         case option::european_call:
           result = underlying_price * quotek::quant::N(d1) - modelized_option.strike * exp( -1 * safe_interest_rate * modelized_option.expires ) * quotek::quant::N(d2);
           break;
          case option::european_put:
            result = -1 * underlying_price * quotek::quant::N(-1*d1) + modelized_option.strike * exp( -1 * safe_interest_rate * modelized_option.expires ) * quotek::quant::N(-1 * d2) ;
            break;
          case option::us_call:
            result = underlying_price * quotek::quant::N(d1) - modelized_option.strike * exp( -1 * safe_interest_rate * modelized_option.expires ) * quotek::quant::N(d2);
            break;
           case option::us_put:
             result = -1 * underlying_price * quotek::quant::N(-1*d1) + modelized_option.strike * exp( -1 * safe_interest_rate * modelized_option.expires ) * quotek::quant::N(-1 * d2) ;
             break;
          default:
            break;

       }

       return result;
     }
     
     float blackscholes::compute_d1() {
     
      float sq_t = sqrt(modelized_option.expires);
      this->d1 = (log( underlying_price / modelized_option.strike ) +  safe_interest_rate * modelized_option.expires )/( implied_volatility * sq_t) + 0.5 * implied_volatility * sq_t;
      return this->d1;
     }

     float blackscholes::compute_d2() {
       
       float sq_t = sqrt(modelized_option.expires);
       this->d2 = this->d1 - (implied_volatility * sq_t);
       return this->d2;
     }
  }
}
