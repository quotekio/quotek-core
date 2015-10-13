/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include <quotek/broker.hpp>
#include <quotek/generators.hpp>
#include <quotek/quant.hpp>
#include <iostream>

void test_uniform() {

  quotek::data::records uni = quotek::rand::generators::uniform(10,4000,5000);

  assert( uni.size() == 10 );
  assert( quotek::quant::min(uni) >= 4000 );
  assert( quotek::quant::max(uni) <= 5000 );

}

void test_normal() {

  quotek::data::records uni = quotek::rand::generators::normal(10,5000,20);
  assert( uni.size() == 10 );

}

void test_lognormal() {

  quotek::data::records uni = quotek::rand::generators::lognormal(10,3,.0000000001);
  assert( uni.size() == 10 );
  

}

int main() {
  test_uniform();
  test_normal();
  test_lognormal();
}
