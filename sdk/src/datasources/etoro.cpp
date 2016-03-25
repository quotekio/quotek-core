/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS
http://www.quotek.io
*/

#include "etoro.hpp"
#include <rapidjson/document.h>

namespace quotek {

  namespace datasource {

    std::string etoro::base_url = "https://openbook.etoro.com/API";

  	etoro::etoro() {}
  	etoro::~etoro() {}

  	std::vector<quotek::datasource::eto_user> etoro::ranking(int period, 
  		                       std::string risk_level,
  		                       std::string sort_type) {

  		std::vector<quotek::datasource::eto_user> result;

  		quotek::http http1;
        std::map<std::string, std::string > args;

        std::stringstream ss;
        ss << period;

        args["period"] = ss.str();
        args["riskLevel"] = risk_level;
        args["sort"] = sort_type; 

  		std::string rdata = http1.get(base_url + "/Rankings/",args);

  		rapidjson::Document d;
  		d.Parse<0>(rdata.c_str());

        if (! d.HasParseError() ) {

          if (  d.HasMember("Users")  ) {
            for ( int i=0;i< d["Users"].Size(); i++ ) {

              eto_user e1;

              e1.username = d["Users"][i]["Username"].GetString();
              e1.max_drawdown = d["Users"][i]["MaxDrawDown"].GetDouble();
              e1.profitable_weeks = d["Users"][i]["ProfitableWeeks"].GetDouble();
              e1.gain = d["Users"][i]["Gain"].GetDouble();
              e1.copiers = d["Users"][i]["Copiers"].GetInt();
              e1.rank = d["Users"][i]["Rank"].GetInt();

              result.emplace_back(e1);

            }

          }
        }

  		return result;

  	}

    
    std::vector< std::vector<quotek::datasource::eto_user> > etoro::top() {

      std::vector< std::vector<quotek::datasource::eto_user> > result;
      quotek::http http1;
      std::string nurl = base_url + "/Rankings/TopPerformers";

      std::string rdata = http1.get(nurl);
      //std::cout << rdata << std::endl;

      rapidjson::Document d;
      d.Parse<0>(rdata.c_str());
      if (! d.HasParseError() ) {

        if (  d.HasMember("TopPerformers") ) {

          for (int i=0;i< d["TopPerformers"].Size(); i++ ) {

            std::vector<eto_user> tlist;
            for (int j=0;j< d["TopPerformers"][i]["Users"].Size();j++) {

              eto_user e1;
              e1.username = d["TopPerformers"][i]["Users"][j]["User"]["Username"].GetString();
              e1.gain = d["TopPerformers"][i]["Users"][j]["Ranking"]["Gain"].GetDouble();
              e1.rank = d["TopPerformers"][i]["Users"][j]["Ranking"]["Rank"].GetInt();
              e1.copiers = d["TopPerformers"][i]["Users"][j]["Followers"].GetInt();

              e1.max_drawdown = 0;
              e1.profitable_weeks = 0;

              tlist.emplace_back(e1);

            }
            
            result.emplace_back(tlist);

          }

        }
      }

      return result;
    }
    

  	std::vector<quotek::core::position> etoro::positions(std::string username,
  		                                          std::string pos_type,
  		                                          std::string pos_time) {

  		std::vector<quotek::core::position> result;
        quotek::http http1;
        std::string nurl = base_url + "/Users/Positions/" +  username + "/" + pos_type + "/" + pos_time; 

        std::string rdata = http1.get(nurl);

        rapidjson::Document d;

        d.Parse<0>(rdata.c_str());

        if (! d.HasParseError() ) {

          if ( d.HasMember("Positions") ) {
            
            for (int i=0;i< d["Positions"]["Positions"].Size();i++) {

              quotek::core::position p;
              
              p.size = (  d["Positions"]["Positions"][i]["IsBuy"].GetBool() == true  ) ? 1 : -1;
              
              std::string openstr = d["Positions"]["Positions"][i]["OpenRate"].GetString();
              std::string pnlstr = d["Positions"]["Positions"][i]["Gain"].GetString();

              p.open = atof( openstr.c_str() );
              p.pnl = atof(  pnlstr.c_str());
              

              /***** MISSING IN QUOTEK POSITION STRUCT *****
              if ( ! d["Positions"]["Positions"][i]["CloseRate"].IsNull() ) {
                p.close = d["Positions"]["Positions"][i]["CloseRate"].GetDouble();
              }
              else p.close = 0; */
              
              if ( ! d["Positions"]["Positions"][i]["StopLoss"].IsNull() ) {
              	std::string stopstr = d["Positions"]["Positions"][i]["StopLoss"].GetString();
                p.stop = atof(stopstr.c_str());
              }
              else p.stop = 0;

              if ( ! d["Positions"]["Positions"][i]["TakeProfit"].IsNull() ) {
              	std::string limitstr = d["Positions"]["Positions"][i]["TakeProfit"].GetString();
                p.limit = atof(limitstr.c_str());
              }
              else p.limit = 0;
              

              if ( ! d["Positions"]["Positions"][i]["OpenDate"].IsNull() ) {
                p.open_date = quotek::core::time::p_strptime(d["Positions"]["Positions"][i]["OpenDate"].GetString(), "%Y,%m,%d,%H,%M,%S" );
              }
              else p.open_date = 0;
              
              if ( ! d["Positions"]["Positions"][i]["CloseDate"].IsNull() ) {
                p.close_date = quotek::core::time::p_strptime( d["Positions"]["Positions"][i]["CloseDate"].GetString(),"%Y,%m,%d,%H,%M,%S" );
              }
              else p.close_date = 0;

              p.ticket_id = "none";
              p.asset_name = d["Positions"]["Positions"][i]["Market"]["Name"].GetString();

              result.emplace_back(p);

            } 

          }

        }

        return result;

  	}

   
    std::vector<std::string> etoro::symbols(std::string category) {
  
      std::vector<std::string> result;

      quotek::http http1;
      std::map<std::string, std::string > args;
      args["category"] = category;

      std::string rdata = http1.get( base_url + "/Markets/Category",args);

      rapidjson::Document d;
  	  d.Parse<0>(rdata.c_str());

      if (! d.HasParseError() ) {

        if ( d.HasMember("Symbols") ) {

          for (int i=0;i< d["Symbols"].Size();i++) {
            result.emplace_back( d["Symbols"][i]["Symbol"]["MarketName"].GetString() );
          }

        }

      }

      return result;

    }

    
    std::vector<float> etoro::insight(std::string symbol, std::string category) {

      std::vector<float> result;

      quotek::http http1;
      std::map<std::string, std::string > args;
      args["category"] = category;

      std::string rdata = http1.get( base_url + "/Markets/Category",args);

      rapidjson::Document d;
  	  d.Parse<0>(rdata.c_str());

      if (! d.HasParseError() ) {

        if ( d.HasMember("Symbols") ) {

          for (int i=0;i< d["Symbols"].Size();i++) {
            
            std::string mname =  d["Symbols"][i]["Symbol"]["MarketName"].GetString();

            if (mname == symbol) {

              float svar = d["Symbols"][i]["SymbolRatesModel"]["SentimentPercent"].GetDouble();
              std::string sway = d["Symbols"][i]["SymbolRatesModel"]["SentimentType"].GetString();

              if (sway == "Buying") {
                result.emplace_back(svar);
                result.emplace_back(100.0 - svar);

              }
              else {
                result.emplace_back(100.0 - svar);
                result.emplace_back(svar);
                
              }

              return result;
            }

          }

        }

      }

      return result;

    }

  }
}
