/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#ifndef TWITTER_HPP
#define TWITTER_HPP

#include <string>
#include "../datasource.hpp"
#include "../news.hpp"

namespace quotek {

  namespace datasource {
   
    /** Twitter is a class that allows to fetch twitter content from the regular API. */

    class twitter: public datasource {

      public:

        /** twitter class constructor */
      	twitter(std::string consumer_key, 
                std::string consumer_secret,
                std::string token,
                std::string token_secret);

      	/** twitter class destructor */
      	~twitter();

        /** fetches the last tweets of given user.
         *  @param screen_name: The name of account to fetch tweets for.
         *  @param nb_tweets: The number of tweets to fetch (sorted by descending date)
      	 *  @return a vector of quotek news objects.
         */     
      	std::vector<quotek::data::news> fetch(std::string screen_name,
                                              int nb_tweets);

      private:

        void* tw_handler;

    };

  }
}

#endif
