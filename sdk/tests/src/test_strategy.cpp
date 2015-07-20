/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#include <quotek/strategy.hpp>
#include <iostream>

void test_order(quotek::core::strategy& s) {

  s.order("foobar");

  std::string gen_order;
  s.orders_queue.pop(gen_order, false);
  assert( gen_order == "foobar");

}

void test_log(quotek::core::strategy& s) {

  s.log("foobar");

  std::string gen_log;
  s.log_queue.pop(gen_log, false);
  assert( gen_log == "foobar");
}

int main() {

  std::vector<quotek::data::record> recs;
  std::map<std::string, quotek::data::any> store;
  std::vector<quotek::core::position> portfolio;

  quotek::core::strategy s(recs,store,portfolio);
  test_order(s);
  test_log(s);

}
