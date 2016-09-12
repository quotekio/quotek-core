/*
Quotek Strategies SDK 3.0
Copyright 2013-2016 Quotek SAS
*/

#include "imap.hpp"

size_t imap_curl_writehandle(void *ptr,
	                      size_t size,
	                      size_t nmemb,
	                      std::string& stream) {

    if (ptr != NULL) {
      std::string temp(static_cast<const char*>(ptr), size * nmemb);
      stream += temp;
    }
    return size*nmemb;
}

namespace quotek {

  imap::imap(std::string imap_server, 
            std::string username, 
            std::string password,
            const bool secure, 
            const bool ssl_verify) {

    this->imap_server = imap_server;
    this->username = username;
    this->password = password;
    this->secure = secure;
    this->ssl_verify = ssl_verify;

    ch = curl_easy_init();
    
    this->root_url = "imap://" + this->imap_server;

    if (this->secure == true) this->root_url = "imaps://" + this->imap_server + ":993";

    curl_easy_setopt(ch,CURLOPT_SSL_VERIFYPEER, 0);    

    if (this->ssl_verify == true) {
      curl_easy_setopt(ch,CURLOPT_SSL_VERIFYPEER, 1);
    }


  }

  imap::~imap() {

  }

  
  /*
  std::vector<std::string> imap::list(const std::string folder = "INBOX") {

  }*/

  quotek::data::email imap::fetch(const std::string folder, 
  	                                     const std::string uid) {

    quotek::data::email result;
    std::string wdata = "";

    curl_easy_setopt(ch, CURLOPT_USERNAME, this->username.c_str());
    curl_easy_setopt(ch, CURLOPT_PASSWORD, this->password.c_str());

    std::stringstream cur_url;
    cur_url << root_url << "/" << folder << ";UID=" << uid;

    curl_easy_setopt(ch, CURLOPT_URL, cur_url.str().c_str() );

    curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,imap_curl_writehandle);
    curl_easy_setopt(ch,CURLOPT_WRITEDATA,&wdata);

    curl_easy_perform(ch);

    result.data = wdata;
    result.parse();

    return result;


  }

  quotek::data::email imap::fetch_latest(const std::string folder) {
    return this->fetch(folder,"*");
  }



}