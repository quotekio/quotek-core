/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/
#include <quotek/broker.hpp>
#include <quotek/blackscholes.hpp>


void test_blackscholes_callprice() {

  quotek::quant::option vanilla(quotek::quant::option::european_call,
                                "IBM",
                                0,
                                28.5,
                                0.4);

  quotek::quant::blackscholes bla(vanilla,20.4,1,10);
  
  float opt_price = bla.price_option();

  std::cout << 






}

void test_blackscholes_putprice() {

}


int main() {

  test_blackscholes_callprice();
  test_blackscholes_putprice();

  


}
