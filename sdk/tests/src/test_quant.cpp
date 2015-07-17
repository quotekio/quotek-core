/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#include <quotek/quant.hpp>
#include <iostream>

#define EPSILON 0.01

void test_above(std::vector<quotek::data::record>& recs) {

}

void test_min(std::vector<quotek::data::record>& recs) {
  float minval = quotek::quant::min(recs);
  assert(minval == 4144);

}

void test_max(std::vector<quotek::data::record>& recs) {

  float maxval = quotek::quant::max(recs);
  assert(maxval == 4989);

}

void test_average(std::vector<quotek::data::record>& recs) {
  float avg = quotek::quant::average(recs);
  assert( fabs(avg - 4558.56) < EPSILON );
}

void test_variance(std::vector<quotek::data::record>& recs) {
  float var = quotek::quant::variance(recs,false);
  assert( fabs(var - 71192.024) < EPSILON ); 

}

void test_standard_deviation(std::vector<quotek::data::record>& recs) {
  float stddev = quotek::quant::SD(recs,false);
  assert( fabs( stddev - 266.818 ) < EPSILON );
  stddev = quotek::quant::SD(recs,true);
  assert( fabs( stddev - 283 ) < EPSILON );

}


void test_trend_percentage(std::vector<quotek::data::record>& recs) {

  quotek::quant::trend_p trend = quotek::quant::trend_percentages(recs);
  assert( fabs( trend.bull - ( (6.0 / 8.0) * 100 ) ) < EPSILON );
  assert( fabs( trend.bear - ( (2.0 / 8.0) * 100 ) ) < EPSILON );
  assert( trend.neutral == 0 );
  
}

void test_linear_regression(std::vector<quotek::data::record>& recs) {

  quotek::quant::affine af = quotek::quant::linear_regression(recs);

  std::cout << af.a << std::endl;
  std::cout << af.b << std::endl;

}

int main() {

  //creates false record
  
  std::vector<long> tstamps = { 1437143900, 
                                1437143901, 
                                1437143902, 
                                1437143903, 
                                1437143904, 
                                1437143905, 
                                1437143906, 
                                1437143907, 
                                1437143908 };

  std::vector<float> vals = { 4530, 4575, 4890, 4235, 4144, 4356, 4588, 4720, 4989 };


  std::vector<quotek::data::record> r1 = quotek::data::record::values_import(vals);

  for (int i=0;i< r1.size(); i++ ) {
    r1[i].timestamp = tstamps[i];
  }

  test_min(r1);
  test_max(r1);
  test_average(r1);
  test_standard_deviation(r1);
  test_variance(r1);

  test_linear_regression(r1);
  test_trend_percentage(r1);


}
