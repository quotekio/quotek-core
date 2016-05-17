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

void test_macd(quotek::data::records& recs) {

  std::vector<quotek::data::records> macdr = quotek::ta::MACD(recs,12,26,9);

  assert(macdr.size() == 2);
  assert(macdr[0].size() > 0);
  assert(macdr[1].size() > 0);
  assert(macdr[0].size() > macdr[1].size() );
  assert( quotek::quant::cross(macdr[0],macdr[1]) );
  
}

void test_trix() {

  std::vector<float> vf1 = { 10000, 10140, 9960, 9334.5, 10050, 10290, 10560, 10310, 10280, 10210, 10385 };
  std::vector<float> vf2 = { 10000, 10140, 10230, 10410, 10590, 10750, 10920, 11111, 11290, 11399, 11850 };

  quotek::data::records recs1(vf1);
  quotek::data::records recs2(vf2);
  
  quotek::data::records rx1 = quotek::ta::trix(recs1,4);
  quotek::data::records rx2 = quotek::ta::trix(recs2,4);

  assert(rx1.size() > 0);
  assert(rx2.size() > 0);

  for (int i=1;i< 5;i++) {
    assert( rx1[i].value < 0 );
  }

  for (int i=0;i< rx2.size();i++) {
    assert( rx2[i].value > 0 );
  }

}


void test_fibo_retr() {
 
  quotek::data::records recs;
  recs.append(0,0);
  recs.append(1,100);

  quotek::quant::fibo_ret fl = quotek::ta::fibo_retrace(recs);

  assert(fl.p0 == 0);
  assert( fabs( fl.p23 - 23.6) < EPSILON );
  assert(fl.p50 == 50); 
  assert( fabs( fl.p38 - 38.2) < EPSILON );
  assert( fabs( fl.p61 - 61.8) < EPSILON );
  assert(fl.p100 == 100);
  
}

void test_fibo_ext() {
 
  quotek::data::records recs;
  recs.append(0,0);
  recs.append(1,100);

  quotek::quant::fibo_ext fl = quotek::ta::fibo_extension(recs);

  assert(fl.p100 == 100 );
  assert( fabs( fl.p127 - 127.2) < EPSILON );
  assert( fabs( fl.p161 - 161.8) < EPSILON );
  assert( fl.p200 == 200 );
  assert(fabs( fl.p261 - 261.8) < EPSILON );
  
}




int main() {
  quotek::data::records f1 = quotek::rand::generators::normal(100,4500,30);
  
  test_macd(f1);
  test_trix();
  test_fibo_retr();
  test_fibo_ext();

}
