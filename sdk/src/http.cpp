/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS
http://www.quotek.io
*/

#include "http.hpp"

size_t curl_writehandle(void *ptr,
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

  http::http() {
    ch = curl_easy_init();
    headers = NULL;
  }

  http::~http() {
    this->destroy();
  }
  
  void http::destroy() {
   curl_easy_cleanup(ch);
  }

  void http::add_header(std::string header) {
    headers = curl_slist_append(headers, header.c_str());
  }

  std::string http::escape(std::string to_escape) {
    return std::string(curl_easy_escape(ch,to_escape.c_str(),0));
  }


  std::string http::get(std::string url) {

    std::string wdata = "";

    curl_easy_setopt(ch,CURLOPT_URL,url.c_str());
    curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_writehandle);
    curl_easy_setopt(ch,CURLOPT_WRITEDATA,&wdata);
    curl_easy_setopt(ch,CURLOPT_HTTPHEADER,headers);
    curl_easy_perform(ch);

    return wdata;
  }

  std::string http::get(std::string url, std::map<std::string, std::string>& get_params) {

    std::string wdata = "";
    std::string sparams = "?";

    std::map<std::string, std::string >::iterator it;
    int i=0;
    for(it=get_params.begin();it!=get_params.end();it++) {
      sparams += it->first + "=" + it->second;
      if (i != get_params.size() -1) sparams += "&";
      i++;
    }

    url += sparams;
    
    curl_easy_setopt(ch,CURLOPT_URL,url.c_str());
    curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_writehandle);
    curl_easy_setopt(ch,CURLOPT_WRITEDATA,&wdata);
    curl_easy_setopt(ch,CURLOPT_HTTPHEADER,headers);
    curl_easy_perform(ch);

    return wdata;
  


  }

  std::string http::post(std::string url,std::string& pdata) {

    std::string wdata = "";
    curl_easy_setopt(ch,CURLOPT_URL,url.c_str());
    curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_writehandle);
    curl_easy_setopt(ch,CURLOPT_WRITEDATA,&wdata);
    curl_easy_setopt(ch,CURLOPT_POST,1);
    curl_easy_setopt(ch,CURLOPT_POSTFIELDS,pdata.c_str());
    curl_easy_setopt(ch,CURLOPT_HTTPHEADER,headers);
    curl_easy_perform(ch);

    return wdata;
   
  }

  std::string http::post(std::string url, std::map<std::string, std::string>& post_data) {

    std::string wdata = "";
    std::string wpost = "";

    std::map<std::string, std::string >::iterator it;
    int i=0;
    for(it=post_data.begin();it!=post_data.end();it++) {
      wpost += it->first + "=" + it->second;
      if (i != post_data.size() -1) wpost += "&";
      i++;
    }

    curl_easy_setopt(ch,CURLOPT_URL,url.c_str());
    curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_writehandle);
    curl_easy_setopt(ch,CURLOPT_WRITEDATA,&wdata);
    curl_easy_setopt(ch,CURLOPT_POST,1);
    curl_easy_setopt(ch,CURLOPT_POSTFIELDS,wpost.c_str());
    curl_easy_setopt(ch,CURLOPT_HTTPHEADER,headers);
    curl_easy_perform(ch);

    return wdata;

  }

  std::string http::del(std::string url) {

    std::string wdata = "";

    curl_easy_setopt(ch,CURLOPT_URL,url.c_str());
    curl_easy_setopt(ch,CURLOPT_WRITEFUNCTION,curl_writehandle);
    curl_easy_setopt(ch,CURLOPT_WRITEDATA,&wdata);
    curl_easy_setopt(ch,CURLOPT_CUSTOMREQUEST,"DELETE");
    curl_easy_setopt(ch,CURLOPT_HTTPHEADER,headers);
    curl_easy_perform(ch);
    return wdata;
    
  }

}
