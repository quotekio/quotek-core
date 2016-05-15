/*
Quotek Strategies SDK 2.3
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include <quotek/broker.hpp>
#include <quotek/imap.hpp>
#include <quotek/email.hpp>
#include <iostream>

void test_fetch(quotek::imap& i1) {
  
  quotek::data::email e1 = i1.fetch("INBOX","1");
  assert(e1.from != "");
  assert(e1.to != "");
  assert(e1.subject != "");
  assert(e1.body != "");


}

void test_fetch_latest(quotek::imap& i1) {
  quotek::data::email e1 = i1.fetch_latest("INBOX");
  assert(e1.from != "");
  assert(e1.to != "");
  assert(e1.subject != "");
  assert(e1.body != "");
  
}



int main(int argc, char** argv) {

  quotek::imap i1(argv[1], argv[2], argv[3],true, true);

  test_fetch(i1);
  test_fetch_latest(i1);
  

}
