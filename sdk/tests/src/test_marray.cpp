/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include <quotek/broker.hpp>
#include <quotek/marray.hpp>
#include <iostream>
#include <math.h>

#define EPSILON 0.001

void test_marray() {

  quotek::data::array3d<float> m1(3,3,3);

  m1.at(1,1,2) = 3.14;
  assert( fabs(m1.at(1,1,2) - 3.14) < EPSILON );

 
}

int main() {

  test_marray();

}
