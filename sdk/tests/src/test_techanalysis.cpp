/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include <quotek/broker.hpp>
#include <quotek/techanalysis.hpp>
#include <quotek/generators.hpp>
#include <quotek/quant.hpp>
#include <iostream>

#define EPSILON 0.01

void test_macd(std::vector<quotek::data::record>& recs) {

  std::vector<quotek::data::records> macdr = quotek::ta::MACD(recs,12,26,9);

  assert(macdr.size() == 2);
  assert(macdr[0].size() > 0);
  assert(macdr[1].size() > 0);
  assert(macdr[0].size() > macdr[1].size() );
  assert( quotek::quant::crosses(macdr[0].get_data(),macdr[1].get_data()) );
  
}

void test_fibo() {
 
  quotek::data::records recs;
  recs.append(0,0);
  recs.append(1,100);

  quotek::quant::fibo_levels fl = quotek::ta::fibo_retrace(recs.get_data());

  assert(fl.p0 == 0);
  assert( fabs( fl.p23 - 23.6) < EPSILON );
  assert(fl.p50 == 50); 
  assert( fabs( fl.p38 - 38.2) < EPSILON );
  assert( fabs( fl.p61 - 61.8) < EPSILON );
  assert(fl.p100 == 100);
  
}



int main() {
  std::vector<quotek::data::record> f1 = quotek::rand::generators::normal(100,4500,30);
  test_macd(f1);
  test_fibo();

}
