/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include "alchemy.hpp"
#include <rapidjson/document.h>
#include <iostream>

#include <rapidjson/document.h>

namespace quotek {

  namespace ml {

        
        alchemy::alchemy(std::string key, std::string endpoint) {
          this->key = key;
          this->endpoint = endpoint;
        }

        alchemy::~alchemy() {

        }

        quotek::ml::sentiment alchemy::sentiment(std::string payload, 
                                       std::string target, 
                                       std::string format) {

          std::string data = this->query("GetTextSentiment", payload , format);

          quotek::ml::sentiment s1;

          rapidjson::Document d;
          d.Parse<0>(data.c_str());

          if (  d["docSentiment"].IsObject() ) {

            std::string score_str = d["docSentiment"]["score"].GetString();
            std::string type_str = d["docSentiment"]["type"].GetString();

            s1.positive = ( type_str == "positive" ) ? true : false ;
            s1.score = atof(score_str.c_str());
            s1.mixed = ( d["docSentiment"]["mixed"].IsString() ) ? true : false ;
            return s1;
          }

          return s1 ;

        }
        

        std::string alchemy::language(std::string payload,
                             std::string format) {

          std::string data = this->query("GetLanguage", payload , format);          

          rapidjson::Document d;
          d.Parse<0>(data.c_str());
          
          if ( d["language"].IsString() ) {
            return d["language"].GetString();
          }
          return "";
          
        }


        std::string alchemy::query(std::string fct, std::string payload, std::string format) {

          std::string url = this->endpoint;
          quotek::http http_handler;
          std::map<std::string, std::string> pdata;

          pdata["apikey"] = this->key;
          pdata["outputMode"] = "json";
          pdata[format] = payload;

          if ( format == "text" ) {
            url += "/calls/text/Text" + fct;
          }

          else if ( format == "html" ) {
            url += "/calls/html/Html" + fct;
          }

          else if ( format == "url" ) {
            url += "/calls/url/Url" + fct;
          }
          
          return http_handler.post(url,pdata);

        }

    }
}
