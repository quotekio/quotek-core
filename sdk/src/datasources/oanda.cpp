/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include "oanda.hpp"
#include "../http.hpp"
#include <iostream>
#include <regex>

namespace quotek {
	namespace datasource {


  
		oanda::oanda() {

		}

		oanda::~oanda() {

		}

		std::map<std::string, std::vector<float> > oanda::get_fxratios() {

		  std::map<std::string, std::vector<float> > result;

		  quotek::http ht1;
		  std::string ocontent = ht1.get("http://fxtrade.oanda.com/lang/fr/analysis/open-position-ratios");

          std::regex fxblock("<li name=\"(.*?)\\/(.*?)\">([^\\x00]*?)<\\/li>");

          std::regex longblock("<span(.*?)class=\"long-position\">([^\\x00]*?)</span>");
          std::regex shortblock("<span(.*?)class=\"short-position\">([^\\x00]*?)</span>");

          std::smatch m;

          while (std::regex_search (ocontent,m,fxblock)) {
 
             std::vector<float> lsnum;

             std::string blk = m[0];
             std::stringstream name;
             name << m[1] << m[2];

             std::smatch m2;

             if ( std::regex_search(blk,m2,longblock) ) {
               std::string lp = m2[2];
               quotek::core::utils::trim(lp);
               lp[lp.size()-1] = 0x00;
               lsnum.emplace_back( atof( lp.c_str()) );
             }

             if ( std::regex_search(blk,m2,shortblock) ) {
               std::string sp = m2[2];
               quotek::core::utils::trim(sp);
               sp[sp.size()-1] = 0x00;
               lsnum.emplace_back( atof( sp.c_str()) );
             }

             if ( lsnum.size() == 2  ) {
               result[name.str()] = lsnum;
             }

             ocontent = m.suffix().str();
           }

		  return result;

		}


		std::vector<float> oanda::get_ratio(std::string fx_pair) {

		  std::map<std::string, std::vector<float> > r = get_fxratios();
		  return r[fx_pair];

		}
	}
}
