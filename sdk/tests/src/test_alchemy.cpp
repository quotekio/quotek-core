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
std::string taxo_corpus = "Islamic State victories against the Taliban have not weakened the overall insurgency, but have mostly inflicted more chaos and misery upon Afghan civilians.";


void test_sentiment(quotek::nlp::alchemy& ac) {
  
  extern std::string sent_pos;
  extern std::string sent_neg;
  
  quotek::nlp::sentiment s1 = ac.sentiment(sent_pos,"","text");

  assert(s1.score > 0);
  assert(s1.sentimentstr == "positive");
  assert(!s1.mixed);

  s1 = ac.sentiment(sent_neg,"","text") ;

  assert(s1.score < 0);
  assert(s1.sentimentstr == "negative");
  assert(!s1.mixed);

}

void test_relations(quotek::nlp::alchemy& ac) {

  std::vector<quotek::nlp::relation> rels = ac.relations(sent_pos,"text");

  assert(rels.size() == 3) ;

  for (int i=0;i<3;i++) {
    assert(rels[i].subject != "");
    assert(rels[i].action.text != "");
    assert(rels[i].action.tense != "");
    assert(rels[i].object != "");
  }
   assert (rels[0].subject == "I" );
   assert (rels[0].action.text == "enjoy" );
   assert (rels[0].action.tense == "present" );
   assert (rels[0].object == "strawberries" );

}


void test_taxonomy(quotek::nlp::alchemy& ac) {

  extern std::string taxo_corpus;
  std::vector<quotek::nlp::category> cats = ac.taxonomy(taxo_corpus,"text");
  assert(cats.size() > 0);
  assert(cats[0].name == "/law, govt and politics/government");

}


void test_language(quotek::nlp::alchemy& ac) {

  extern std::string sent_pos;
  std::string lang = ac.language(sent_pos, "text");

  assert(lang=="english");

  lang = ac.language(es_corpus,"text");

  assert(lang=="spanish");

}


void test_raw(quotek::nlp::alchemy& ac) {

  extern std::string sent_pos;

  std::map<std::string, std::string> params;

  params["url"] = "http://www.wired.com";
  params["outputMode"] = "json";

  std::string res = ac.raw("URLGetLanguage",params);

  assert(res.find("english") != std::string::npos);

}


int main(int argc, char** argv) {

  quotek::nlp::alchemy ac(argv[1],"http://access.alchemyapi.com");

  test_sentiment(ac); 
  test_language(ac);
  test_relations(ac);
  test_raw(ac);
  test_taxonomy(ac);
  
}
