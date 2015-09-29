/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include <quotek/broker.hpp>
#include <iostream>

void test_buy(strategy& s) {

  quotek::broker* b0 = new quotek::broker(&s);
  b0->buy("CAC40",2,20,20);

  std::string gen_order;
  s.orders_queue.pop(gen_order, false);

  assert( gen_order == "openpos:CAC40:buy:2:20:20");

}

void test_sell(strategy& s) {

  quotek::broker* b0 = new quotek::broker(&s);
  b0->sell("CAC40",2,20,20);

  std::string gen_order;
  s.orders_queue.pop(gen_order, false);

  assert( gen_order == "openpos:CAC40:sell:2:20:20");

}

void test_smartbuy(strategy& s) {

  quotek::broker* b0 = new quotek::broker(&s);
  b0->smartbuy("CAC40",2);

  std::string gen_order;
  s.orders_queue.pop(gen_order, false);
  assert( gen_order == "smartpos:CAC40:buy:2");

}

void test_smartsell(strategy& s) {

  quotek::broker* b0 = new quotek::broker(&s);
  b0->smartsell("CAC40",2);

  std::string gen_order;
  s.orders_queue.pop(gen_order, false);
  assert( gen_order == "smartpos:CAC40:sell:2");
  
}

void test_buy_hedged(strategy& s) {

  quotek::broker* b0 = new quotek::broker(&s);
  b0->buy_hedged("CAC40","EURUSD",2,20,20);

  std::string gen_order;
  s.orders_queue.pop(gen_order, false);

  assert( gen_order == "openpos:CAC40:buy:2:20:20");

  s.orders_queue.pop(gen_order, false);
  assert( gen_order == "openpos:EURUSD:sell:2:20:20");

}


void test_buy_hedged_asym(strategy& s) {

  quotek::broker* b0 = new quotek::broker(&s);
  b0->buy_hedged_asym("CAC40",2,20,20,"EURUSD",5,40,20);

  std::string gen_order;
  s.orders_queue.pop(gen_order, false);

  assert( gen_order == "openpos:CAC40:buy:2:20:20");

  s.orders_queue.pop(gen_order, false);
  assert( gen_order == "openpos:EURUSD:sell:5:40:20");

}


void test_closepos(strategy& s) {

  quotek::broker* b0 = new quotek::broker(&s);
  b0->close_position("DSSJKDSL44");

  std::string gen_order;
  s.orders_queue.pop(gen_order, false);
  assert( gen_order == "closepos:DSSJKDSL44");
  
}

int main() {

  strategy s;
  test_buy(s);
  test_sell(s);
  test_smartbuy(s);
  test_smartsell(s);
  test_buy_hedged(s);
  test_closepos(s);


}
