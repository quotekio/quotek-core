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
#include "../timeutils.hpp"

namespace quotek {

  namespace datasource {
   
    /** Twitter is a class that allows to fetch twitter content from the regular API. */

    class twitter: public datasource {

      public:

        /** twitter class constructor. To get all the constructor parameters, 
         *  just go to https://apps.twitter.com/, create a new app and go to "Keys and Access Tokens".
         *
         *  @param consumer_key: Twitter app consumer key.
         *  @param consumer_secret: Twitter app consumer secret.
         *  @param token: Twitter app token.
         *  @param token_secret: Twitter app token secret.
         */
      	twitter(std::string consumer_key, 
                std::string consumer_secret,
                std::string token,
                std::string token_secret);

      	/** twitter class destructor */
      	~twitter();

        /** fetches the last tweets of given user.
         *
         *  @param screen_name: The name of account to fetch tweets for.
         *  @param nb_tweets: The number of tweets to fetch (sorted by descending date)
      	 *  @return a vector of quotek news objects.
         */     
      	std::vector<quotek::data::news> fetch(std::string screen_name,
                                              int nb_tweets);


        /** searches the tweets matching the term provided as argument.
         *
         *  @param term: term to search in the global tweets feed.
         *  @param nb_tweets: The number of found tweets to retrieve.
         *  @return a vector of quotek news objects matching the search.
         */

        std::vector<quotek::data::news> search(std::string term,
                                               int nb_tweets);

        /** searches the tweets matching the term provided as argument.
         *  This method refines the usual search() method by adding 2 extra
         *  parameters: result_type and language.
         *  @param term: term to search in the global tweets feed.
         *  @param nb_tweets: The number of found tweets to retrieve.
         *  @param result_type: Type of the search result, can be either "popular", "recent" or "mixed".
         *  @param language of the tweets, eg for english only tweets put lang = "en".
         *  @return a vector of quotek news objects matching the search. */

        std::vector<quotek::data::news> search(std::string term,
                                               int nb_tweets,
                                               std::string result_type,
                                               std::string lang);


      private:

        void* tw_handler;

    };

  }
}

#endif
