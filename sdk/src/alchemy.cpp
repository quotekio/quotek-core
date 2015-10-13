/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include "alchemy.hpp"
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <iostream>

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
          s1.score = 0;
          s1.error = false;
          s1.error_message = "";

          rapidjson::Document d;
          d.Parse<0>(data.c_str());

          if ( d.HasParseError() ) {
            s1.error = true;
            s1.error_message = rapidjson::GetParseError_En(d.GetParseError());
            return s1;
          }

          if ( d["status"].GetString() == std::string("OK") ) {

            if ( ! d.HasMember("docSentiment") ) {
              s1.error = true;
              s1.error_message = "Alchemy API returned no sentiment object.";
              return s1;
            }

            std::string score_str = "";
            std::string type_str = "";

            if ( d["docSentiment"].HasMember("score") ) score_str = d["docSentiment"]["score"].GetString();
            if ( d["docSentiment"].HasMember("type") ) type_str = d["docSentiment"]["type"].GetString();

            s1.sentimentstr = type_str;
            s1.score = atof(score_str.c_str());
            if (d["docSentiment"].HasMember("mixed") ) s1.mixed = true;
            else s1.mixed = false;
            return s1;
          }
          
          /* Something went wrong ! */
          else {
            s1.error = true;
            s1.error_message = "Alchemy API returned an error status.";
            return s1;
          }

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

        std::vector<quotek::ml::relation> alchemy::relations(std::string payload, 
                                                             std::string format) {

          std::vector<quotek::ml::relation> result;
          std::string data = this->query("GetRelations",payload,format);

          rapidjson::Document d;
          d.Parse<0>(data.c_str());

          if (d["relations"].IsArray()) {
            for (int i=0;i<d["relations"].Size();i++) {

              quotek::ml::relation r0;

              r0.sentence = d["relations"][i]["sentence"].GetString();
              r0.subject = d["relations"][i]["subject"]["text"].GetString();

              if ( d["relations"][i].HasMember("action") ) {
                r0.action.text =  d["relations"][i]["action"]["text"].GetString();
                r0.action.tense = d["relations"][i]["action"]["verb"]["tense"].GetString();

                if ( d["relations"][i]["action"]["verb"].HasMember("negated") ) {
                  r0.action.negated = true;
                }
                else r0.action.negated = false;
              }
              r0.object = d["relations"][i]["object"]["text"].GetString();

              result.emplace_back(r0);

            }
          }

          return result;
        }


        std::vector<quotek::ml::category> alchemy::taxonomy(std::string payload,
                                                   std::string format) {

          std::vector<quotek::ml::category> result;
          std::string data = this->query("GetRankedTaxonomy",payload,format);

          rapidjson::Document d;
          d.Parse<0>(data.c_str());

          if (d.HasMember("taxonomy")) {

            for (int i=0;i<d["taxonomy"].Size();i++) {

              quotek::ml::category cat0;
              cat0.name = d["taxonomy"][i]["label"].GetString()  ;
              std::string score_str = d["taxonomy"][i]["score"].GetString();
              cat0.score = atof(score_str.c_str()) ;

              if ( d["taxonomy"][i].HasMember("confident")  ) cat0.confident = false;
              else cat0.confident = true;

              result.emplace_back(cat0);

            }
          }
          return result;

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


        std::string alchemy::raw(std::string call,
                                 std::map<std::string, std::string> parameters) {

          std::string url = this->endpoint;
          quotek::http http_handler;

          parameters["apikey"] = this->key;

          if (  call.find("Text") == 0 ) url += "/calls/text/" + call;
          else if (  call.find("HTML") == 0 ) url += "/calls/html/" + call;
          else if (  call.find("URL") == 0 ) url += "/calls/url/" + call;

          return http_handler.post(url,parameters);

        }

    }
}
