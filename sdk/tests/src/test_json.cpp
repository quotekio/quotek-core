/*
Quotek Strategies SDK 2.3
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include <quotek/json.hpp>
#include <quotek/broker.hpp>
#include <iostream>

void test_json_simple(){

  const char* ex = "{\"foo\":1}";
  quotek::json::node* rnode = quotek::json::parser::parse(ex);
  assert(rnode != nullptr);
  assert(rnode->IsObject());

  quotek::json::jobject root = rnode->AsObject();

  assert(root["foo"]->AsNumber() == 1);

}

void test_json_complex() {

  quotek::json::node* rnode = quotek::json::parser::parse("[{\"name\":\"CAC40\",\"columns\":[\"time\",\"sequence_number\",\"spread\",\"value\"],\"points\":[[1460596349,512826800001,5,4488],[1460596350,512826930001,5,4488.8],[1460596351,512826940001,5,4488.8],[1460596352,512827070001,5,4488.8],[1460596353,512827080001,5,4488.8],[1460596354,512827210001,5,4488.8],[1460596355,512827220001,5,4488.8],[1460596356,512827350001,5,4488.8],[1460596357,512827360001,5,4488.1]]}]");

  assert( rnode != nullptr );
  assert( rnode->IsArray() );

  quotek::json::jarray root = rnode->AsArray();

  assert(root[0]->IsObject());

  quotek::json::jobject relem = root[0]->AsObject();
  
  quotek::json::jarray points = relem["points"]->AsArray();

  assert(points.size() == 9);
  
  /*
  for (int i=0;i< points.size();i++ ) {

    quotek::json::jarray p = points[i]->AsArray();
    std::cout << p[3]->AsNumber() << std::endl;
  }
  */
}


int main() {
  test_json_simple();
  test_json_complex();

  exit(0);
}