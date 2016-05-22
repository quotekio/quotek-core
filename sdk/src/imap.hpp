/*
Quotek Strategies SDK 3.0
Copyright 2013-2016 Quotek SAS
*/

#ifndef IMAP_HPP
#define IMAP_HPP

#include <curl/curl.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

#include "email.hpp"


namespace quotek {

  /** IMAP is a simple IMAP client that will allow your strategies to remotely fetch data.
   *  from mailboxes. */

  class imap {

    public:

    	imap(std::string imap_server, 
    		std::string username,
    		std::string password, 
    		const bool secure = false, 
    		const bool ssl_verify = false);

    	~imap();
    	
    	//std::vector<std::string> list(const std::string folder = "INBOX");

    	quotek::data::email fetch(const std::string folder = "INBOX", 
  	                                           const std::string uid = "1");

    	quotek::data::email fetch_latest(const std::string folder = "INBOX");
        
        std::string imap_server;
        std::string username;
        std::string password;
        bool secure;
        bool ssl_verify;



    private:

      /** Internal curl object handler. */
      CURL* ch;

      std::string root_url;


  };

}

#endif