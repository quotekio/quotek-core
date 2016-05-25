/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include <quotek/broker.hpp>
#include <quotek/record.hpp>
#include <quotek/constants.hpp>
#include <iostream>
#include <sstream>
#include <cmath>

#define EPSILON 0.001


void test_construct(quotek::data::records& recs) {
  assert( recs.size() == 1600);
}

void test_down_sample(quotek::data::records& recs) {

  quotek::data::records rtest = recs.down_sample(T_m15,1,"close");

  assert( rtest.size() == 2 );
  assert( rtest[0].value == 899 );
  assert( rtest[1].value == 1599);

  rtest = recs.down_sample(T_m15,1,"HL2");

  assert( rtest.size() == 2 );
  assert( rtest[0].value == 449.5 );
  assert( rtest[1].value == 1249.5 );

  rtest = recs.down_sample(T_m15,1,"typical");

  assert( rtest.size() == 2 );
  assert( fabs(rtest[0].value - 599.333) < EPSILON );
  assert( rtest[1].value == 1366 );

  rtest = recs.down_sample(T_m15,1,"OHLC4");

  assert( rtest.size() == 2 );
  assert( rtest[0].value == 449.5 );
  assert( rtest[1].value == 1249.5 );

  //ERR test.
  rtest = recs.down_sample(T_m15,1,"bla");
  assert( rtest.size() == 0 );

}

void test_stream(quotek::data::records& recs) {

  stringstream ss;
  ss << recs;
  
}


void test_tovector(quotek::data::records& recs) {

  Eigen::VectorXd v = recs.to_vector();

  assert( v.rows() == 1600  );
  assert(v.row(1599)[0] = 1599);

}

void test_tomatrix(quotek::data::records& recs) {

  Eigen::MatrixXd m = recs.to_matrix(true);
  assert( m.rows() == 1600 && m.cols() == 3 );

  assert(m.row(1599).col(1)[0] == 1599);


}


int main() {

  std::vector<std::tuple<long,float,float>> rdata;
  //populates rdata
  for ( int i= 0 ; i< 1600;i++) {
    std::tuple<long,float,float> t = std::make_tuple(i, i/1.0 ,0);
    rdata.emplace_back(t);
  }
  quotek::data::records r1(rdata);

  test_construct(r1);
  test_down_sample(r1);
  test_stream(r1);

  exit(0);

}
