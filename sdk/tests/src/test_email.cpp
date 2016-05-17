/*
Quotek Strategies SDK 2.3
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include <quotek/broker.hpp>
#include <quotek/email.hpp>
#include <iostream>
#include <fstream>

void test_parse_email(std::string email) {

  quotek::data::email e1(email);
  e1.parse();

  assert( e1.from == "\"Gmail Team\" <mail-noreply@google.com>"   );
  assert( e1.to == "\"Foobar\" <foo@bar.com>" );
  assert( e1.subject == "Gmail is different. Here's what you need to know." );
  assert( e1.date == "Thu, 23 Nov 2006 14:55:13 -0800");
  assert( e1.body.size() > 50  );

}


int main(int argc, char** argv) {

  std::string email_data = "";

  std::ifstream emf("fixtures/email.txt");

  std::string line;

  while(emf) {
    line = "";
    std::getline(emf, line);
    email_data += line + "\n";
  }

  test_parse_email(email_data);

  exit(0);

}
