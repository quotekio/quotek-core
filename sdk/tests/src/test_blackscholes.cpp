/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include <quotek/broker.hpp>
#include <quotek/blackscholes.hpp>
#include <iostream>

#define EPSILON 0.01


void test_blackscholes_callprice(quotek::quant::blackscholes& bla) {

  float opt_price = bla.price_option();
  assert( fabs( opt_price - 23.693 ) < EPSILON);

}

void test_blackscholes_callgreeks(quotek::quant::blackscholes& bla) {

  bla.compute_greeks();
  assert( fabs( bla.greeks.delta - 0.758786 ) < EPSILON);
  assert( fabs( bla.greeks.gamma - 0.00471343 ) < EPSILON);
  assert( fabs( bla.greeks.vega - 6.87217 ) < EPSILON);
  assert( fabs( bla.greeks.theta - -53.4032 ) < EPSILON);
  assert( fabs( bla.greeks.rho - 2.50851) < EPSILON);
}


void test_blackscholes_putprice(quotek::quant::blackscholes& bla) {

  float opt_price = bla.price_option();
  assert( fabs( opt_price - 25.891 ) < EPSILON);
}

void test_blackscholes_putgreeks(quotek::quant::blackscholes& bla) {

  bla.compute_greeks();
  assert( fabs( bla.greeks.delta - -0.241214 ) < EPSILON);
  assert( fabs( bla.greeks.gamma - 0.00471343 ) < EPSILON);
  assert( fabs( bla.greeks.vega - 6.87217 ) < EPSILON);
  assert( fabs( bla.greeks.theta - -6.20555 ) < EPSILON);
  assert( fabs( bla.greeks.rho - -8.81893) < EPSILON);

}

int main() {

  quotek::quant::option vanilla_opt(quotek::quant::option::european_call,
                                "ASSET",
                                0,
                                60,
                                0.24);

  quotek::quant::blackscholes bla(vanilla_opt,45,1,3);

  test_blackscholes_callprice(bla);
  test_blackscholes_callgreeks(bla);

  bla.modelized_option.type = quotek::quant::option::european_put;
  test_blackscholes_putprice(bla);
  test_blackscholes_putgreeks(bla);

  
  


}
