/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include "yahoofinance.hpp"
#include <rapidjson/document.h>

namespace quotek {

  namespace datasource {

      yahoofinance::yahoofinance() {
        this->source = "https://query.yahooapis.com/v1/public/yql?q=";
      }
      yahoofinance::~yahoofinance() {

      }

      std::vector<quotek::data::history> yahoofinance::history(std::string symbol,
      		                                                     std::string from, 
      		                                                     std::string to) {

      	std::vector<quotek::data::history> result;

        std::string yql = "select * from yahoo.finance.historicaldata where symbol = \"" + 
                          symbol + 
                          "\" and startDate = \"" + 
                          from + 
                          "\" and endDate = \"" +
                          to +
                          "\"";

        quotek::http* h1 = new quotek::http();

        std::string data = h1->get(this->source + h1->escape(yql) + "&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys&format=json");
        rapidjson::Document d;
        d.Parse<0>(data.c_str());

        if (d["query"].IsObject()) {
          if ( d["query"]["results"].IsObject() ) {
            if ( d["query"]["results"]["quote"].IsArray() ) {

              for ( int i=0; i< d["query"]["results"]["quote"].Size();i++ ) {

                std::string open_str = d["query"]["results"]["quote"][i]["Open"].GetString();
                std::string close_str = d["query"]["results"]["quote"][i]["Close"].GetString();
                std::string high_str = d["query"]["results"]["quote"][i]["High"].GetString();
                std::string low_str = d["query"]["results"]["quote"][i]["Low"].GetString();

                float open = atof(open_str.c_str());
                float close = atof(close_str.c_str());
                float high = atof(high_str.c_str());
                float low = atof(low_str.c_str());

                result.emplace_back( quotek::data::history(0,open,close,high,low));
              }
            }
          } 
        }
      	return result;
      }

      quotek::data::records yahoofinance::close_history(std::string symbol,
      		                                std::string from, 
      		                                std::string to) {

        quotek::data::records result;

        std::string yql = "select * from yahoo.finance.historicaldata where symbol = \"" + 
                          symbol + 
                          "\" and startDate = \"" + 
                          from + 
                          "\" and endDate = \"" +
                          to +
                          "\"";

        quotek::http* h1 = new quotek::http();

        std::string data = h1->get(this->source + h1->escape(yql) + "&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys&format=json");
        rapidjson::Document d;
        d.Parse<0>(data.c_str());

        if (d["query"].IsObject()) {
          if ( d["query"]["results"].IsObject() ) {
            if ( d["query"]["results"]["quote"].IsArray() ) {

              for ( int i=0; i< d["query"]["results"]["quote"].Size();i++ ) {
                std::string close_str = d["query"]["results"]["quote"][i]["Close"].GetString();
                float close = atof(close_str.c_str());
                result.append(0,close,0);
              }
            }
          } 
        }
        return result;
     }

     std::string yahoofinance::CAC40 = "^FCHI";
     std::string yahoofinance::DAX = "^GDAXI";
     std::string yahoofinance::DOW = "^DJI";
     std::string yahoofinance::SP500 = "^GSPC";
     std::string yahoofinance::NASDAC = "^NDX";
     std::string yahoofinance::NIKKEI = "^N225";

  }
}
