/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include <quotek/strategy.hpp>
#include <iostream>

void test_order(strategy& s) {

  s.order("foobar");

  std::string gen_order;
  s.orders_queue.pop(gen_order, false);
  assert( gen_order == "foobar");

}

void test_log(strategy& s) {

  s.log("foobar");

  std::string gen_log;
  s.log_queue.pop(gen_log, false);
  assert( gen_log == "foobar");
}

int main() {

  strategy s;
  test_order(s);
  test_log(s);

  exit(0);

}
