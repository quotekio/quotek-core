/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include <quotek/broker.hpp>
#include <quotek/record.hpp>
#include <quotek/constants.hpp>
#include <iostream>
#include <cmath>

#define EPSILON 0.001


void test_construct(quotek::data::records& recs) {
  assert( recs.size() == 1600);
}

void test_down_sample(quotek::data::records& recs) {

  quotek::data::records rtest = recs.down_sample(m15,1,"close");

  assert( rtest.size() == 2 );
  assert( rtest[0].value == 899 );
  assert( rtest[1].value == 1599);

  rtest = recs.down_sample(m15,1,"HL2");

  assert( rtest.size() == 2 );
  assert( rtest[0].value == 449.5 );
  assert( rtest[1].value == 1249.5 );

  rtest = recs.down_sample(m15,1,"typical");

  assert( rtest.size() == 2 );
  assert( fabs(rtest[0].value - 599.333) < EPSILON );
  assert( rtest[1].value == 1366 );

  rtest = recs.down_sample(m15,1,"OHLC4");

  assert( rtest.size() == 2 );
  assert( rtest[0].value == 449.5 );
  assert( rtest[1].value == 1249.5 );

  //ERR test.
  rtest = recs.down_sample(m15,1,"bla");
  assert( rtest.size() == 0 );

}


int main() {

  std::vector<std::tuple<long,float,float>> rdata;
  //populates rdata
  for ( int i= 0 ; i< 1600;i++) {
    std::tuple<long,float,float> t = { i, i/1.0,0 };
    rdata.emplace_back(t);
  }
  quotek::data::records r1(rdata);

  test_construct(r1);
  test_down_sample(r1);




}
