/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS
*/

#include "email.hpp"
#include "utils.hpp"
#include <iostream>

namespace quotek {

  namespace data {

	email::email() {

	  this->from = "";
	  this->to  = "";
	  this->subject = "";
	  this->date = "";
	  this->body = "";

	  this->data = "";

	}


	email::email(std::string data) {

	  this->from = "";
	  this->to  = "";
	  this->subject = "";
	  this->date = "";
	  this->body = "";

	  this->data = data;

	}

    email::~email() {

    }

	int email::parse() {

      std::regex from_regex("^From:(.*)$",  std::regex::ECMAScript | std::regex::icase);
      std::regex to_regex("^To:(.*)$",  std::regex::ECMAScript | std::regex::icase);
      std::regex date_regex("^Date:(.*)$", std::regex::ECMAScript | std::regex::icase);
      std::regex subject_regex("^Subject:(.*)$", std::regex::ECMAScript | std::regex::icase);

      std::vector<std::string> sdata = quotek::core::utils::split(this->data,'\n');

      int bstart = 0;

      for (int i=0;i<sdata.size();i++) {

        std::smatch sm;
        if (std::regex_match (sdata[i],sm,from_regex)) {
          this->from = sm[1];
        }

        else if (std::regex_match (sdata[i],sm,to_regex)) {
          this->to = sm[1];
        }

        else if (std::regex_match (sdata[i],sm,subject_regex)) {
          this->subject = sm[1];
        }

        else if (std::regex_match (sdata[i],sm,date_regex)) {
          this->date = sm[1];
        }

        else if (sdata[i] == "") {
          bstart = i+1;
          break;
        }

      }

      //gets body
      for (int i=bstart;i<sdata.size();i++) {
        this->body += sdata[i] + "\n";
      }

      quotek::core::utils::trim(this->from);
      quotek::core::utils::trim(this->to);
      quotek::core::utils::trim(this->subject);
      quotek::core::utils::trim(this->date);

	  return 0;
	}

	int email::parse(std::string data) {
	  this->data = data;
	  return this->parse();
	}

  }
}


