/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

//WARNING: if you run this test with your own API Key, it will consume API Calls on your quotas !

#include <quotek/broker.hpp>
#include <quotek/alchemy.hpp>
#include <iostream>

std::string sent_pos = "I really enjoy strawberries, this is the most delicious food i ever ate.";
std::string sent_neg = "I hate strawberries, these totally suck";

std::string es_corpus = "El coordinador general de Convergencia mantiene que la independencia mejorara las condiciones de vida";


void test_sentiment(quotek::ml::alchemy& ac) {
  
  extern std::string sent_pos;
  extern std::string sent_neg;
  
  quotek::ml::sentiment s1 = ac.sentiment(sent_pos,"","text");

  assert(s1.score > 0);
  assert(s1.positive);
  assert(!s1.mixed);

  s1 = ac.sentiment(sent_neg,"","text") ;

  assert(s1.score < 0);
  assert(!s1.positive);
  assert(!s1.mixed);

}

void test_relations(quotek::ml::alchemy& ac) {

  std::vector<quotek::ml::relation> rels = ac.relations(sent_pos,"text");


}


void test_language(quotek::ml::alchemy& ac) {

  extern std::string sent_pos;
  std::string lang = ac.language(sent_pos, "text");

  assert(lang=="english");

  lang = ac.language(es_corpus,"text");

  assert(lang=="spanish");

}

int main(int argc, char** argv) {

  quotek::ml::alchemy ac(argv[1],"http://access.alchemyapi.com");

  test_sentiment(ac);
  test_language(ac);

  


}
