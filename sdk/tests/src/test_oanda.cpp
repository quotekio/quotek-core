/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include <quotek/broker.hpp>
#include <quotek/datasources/oanda.hpp>
#include <iostream>

void test_get_ratio() {

  quotek::datasource::oanda o1;
  std::vector<float> v1 = o1.get_ratio("EURUSD");
  assert(v1.size() > 0);
  assert(v1[0] > 0);
  assert(v1[1] > 0);
    
}


int main() {

  test_get_ratio();

}
