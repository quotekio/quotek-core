/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/
#include <quotek/broker.hpp>
#include <iostream>

void test_buy(quotek::core::strategy& s) {

  quotek::broker* b0 = new quotek::broker(s);
  b0->buy("CAC40",2,20,20);

  std::string gen_order;
  s.orders_queue.pop(gen_order, false);

  assert( gen_order == "openpos:CAC40:buy:2:20:20");

}

void test_sell(quotek::core::strategy& s) {

  quotek::broker* b0 = new quotek::broker(s);
  b0->sell("CAC40",2,20,20);

  std::string gen_order;
  s.orders_queue.pop(gen_order, false);

  assert( gen_order == "openpos:CAC40:sell:2:20:20");

}

void test_smartbuy(quotek::core::strategy& s) {

  quotek::broker* b0 = new quotek::broker(s);
  b0->smartbuy("CAC40",2);

  std::string gen_order;
  s.orders_queue.pop(gen_order, false);
  assert( gen_order == "smartpos:CAC40:buy:2");

}

void test_smartsell(quotek::core::strategy& s) {

  quotek::broker* b0 = new quotek::broker(s);
  b0->smartsell("CAC40",2);

  std::string gen_order;
  s.orders_queue.pop(gen_order, false);
  assert( gen_order == "smartpos:CAC40:sell:2");
  
}

void test_buy_hedged(quotek::core::strategy& s) {

  quotek::broker* b0 = new quotek::broker(s);
  b0->buy_hedged("CAC40","EURUSD",2,20,20);

  std::string gen_order;
  s.orders_queue.pop(gen_order, false);

  assert( gen_order == "openpos:CAC40:buy:2:20:20");

  s.orders_queue.pop(gen_order, false);
  assert( gen_order == "openpos:EURUSD:sell:2:20:20");

}


void test_buy_hedged_asym(quotek::core::strategy& s) {

  quotek::broker* b0 = new quotek::broker(s);
  b0->buy_hedged_asym("CAC40",2,20,20,"EURUSD",5,40,20);

  std::string gen_order;
  s.orders_queue.pop(gen_order, false);

  assert( gen_order == "openpos:CAC40:buy:2:20:20");

  s.orders_queue.pop(gen_order, false);
  assert( gen_order == "openpos:EURUSD:sell:5:40:20");

}


void test_closepos(quotek::core::strategy& s) {

  quotek::broker* b0 = new quotek::broker(s);
  b0->close_position("DSSJKDSL44");

  std::string gen_order;
  s.orders_queue.pop(gen_order, false);
  assert( gen_order == "closepos:DSSJKDSL44");
  
}

int main() {

  quotek::data::records recs;
  std::map<std::string, quotek::data::any> store;
  std::vector<quotek::core::position> portfolio;

  quotek::core::strategy s(recs,store,portfolio);
  test_buy(s);
  test_sell(s);
  test_smartbuy(s);
  test_smartsell(s);
  test_buy_hedged(s);
  test_closepos(s);


}
