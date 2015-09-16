/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include <quotek/broker.hpp>
#include <quotek/position.hpp>
#include <iostream>

void test_create() {

  quotek::core::position p;
  assert( p.stats != NULL );

}

void test_set_vstop() {

  quotek::core::position p;
  p.set_vstop(50);
  p.set_vstop(30);

  assert(p.get_vstop() == 30);

  assert(p.stats->vstop_increments == 1);
  assert(p.stats->vstop_decrements == 1);

}

void test_vshift() {

  quotek::core::position p;
  p.set_vstop(30);
  p.set_vlimit(30);

  p.vshift(10);
  assert(p.get_vstop() == 20);
  assert(p.get_vlimit() == 40);

  assert( p.stats->vstop_decrements == 1 );
  assert( p.stats->vlimit_increments == 1 );

}

void test_set_vlimit() {

  quotek::core::position p;
  p.set_vlimit(50);
  p.set_vlimit(30);

  assert(p.get_vlimit() == 30);
  assert(p.stats->vlimit_increments == 1);
  assert(p.stats->vlimit_decrements == 1);
}

int main() {

  test_create();
  test_set_vstop();
  test_set_vlimit();

}
