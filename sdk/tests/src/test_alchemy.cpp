/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS

WARNING: if you run this test with your own API Key, it will consume API Calls on your quotas !

*/
#include <quotek/broker.hpp>
#include <quotek/alchemy.hpp>
#include <iostream>

void test_sentiment(quotek::ml::alchemy& ac) {

  std::string sent_pos = "I really enjoy strawberries, this is the most delicious food i ever ate.";
  std::string sent_neg = "I hate strawberries, these totally suck";

  quotek::ml::sentiment s1 = ac.sentiment(sent_pos,"","text");

  assert(s1.score > 0);
  assert(s1.positive);
  assert(!s1.mixed);

  s1 = ac.sentiment(sent_neg,"","text") ;

  assert(s1.score < 0);
  assert(!s1.positive);
  assert(!s1.mixed);

}

int main(int argc, char** argv) {

  quotek::ml::alchemy ac(argv[1],"http://access.alchemyapi.com");

  test_sentiment(ac);

}
