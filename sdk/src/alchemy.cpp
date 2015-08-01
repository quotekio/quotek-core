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

          rapidjson::Document d;
          d.Parse<0>(data.c_str());

          if (  d["docSentiment"].IsObject() ) {

            std::string score_str = d["docSentiment"]["score"].GetString();
            std::string type_str = d["docSentiment"]["type"].GetString();

            bool pos = ( type_str == "positive" ) ? true : false ;
            float score = atof(score_str.c_str());
            bool mixed = ( d["docSentiment"]["mixed"].IsString() ) ? true : false ;
            return quotek::ml::sentiment(score, pos, mixed);
          }

          return quotek::ml::sentiment(0,false,false);

        }

        std::string alchemy::ranked_taxonomy(std::string payload, 
                                             std::string target, 
                                             std::string format) {

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
