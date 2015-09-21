/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#include "twitter.hpp"
#include "../twitcurl/twitcurl.h"
#include <rapidjson/document.h>
#include <iostream>

namespace quotek {

  namespace datasource {
   
    twitter::twitter(std::string consumer_key, 
                     std::string consumer_secret,
                     std::string token,
                     std::string token_secret) {

      std::string reply;

      tw_handler = new twitCurl();
      twitCurl* twh = (twitCurl*) tw_handler;

      twh->getOAuth().setConsumerKey(consumer_key);
      twh->getOAuth().setConsumerSecret(consumer_secret);

      twh->getOAuth().setOAuthTokenKey(token);
      twh->getOAuth().setOAuthTokenSecret(token_secret);

      if( ! twh->accountVerifyCredGet() ) {
        std::cout << "ERROR: cannot authenticate to twitter API" << std::endl;
      }

    }

      	
    twitter::~twitter() {

    }

    std::vector<quotek::data::news> twitter::fetch(std::string screen_name,
                                                   int nb_tweets) {

      std::vector<quotek::data::news> result;
      std::string reply;

      twitCurl* twh = (twitCurl*) tw_handler;

      twh->timelineUserGet( true, true, nb_tweets, screen_name, false );
      twh->getLastWebResponse(reply);

      rapidjson::Document d;
      d.Parse<0>(reply.c_str());

      if ( d.IsArray() ) {

        for (int i=0;i<d.Size();i++) {

          quotek::data::news n;
          n.content = d[i]["text"].GetString();

          result.emplace_back(n);
          
        }
      }

      return result;
    }

  }
}
