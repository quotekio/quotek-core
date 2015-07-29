/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#ifndef RSS_HPP
#define RSS_HPP

#include <string>
#include "../datasource.hpp"
#include "../http.hpp"
#include "../news.hpp"

namespace quotek {

  namespace datasource {
   
    /** rss is a class which allows to fetch rss feeds inside quotek strategies in order to
     *  process them (and potentially dedude positions to take).
     */
    class rss: public datasource {

      public:

        /** rss class constructor */
      	rss(std::string source);

      	/** rss class destructor */
      	~rss();

        /** fetches the the RSS stream passed inside the RSS object constructor. 
         *  @param nb_news: The number of news to fetch (sorted by descending date)
      	 *  @return a vector of quotek news objects.
         */

      	std::vector<quotek::data::news> fetch(int nb_news);

    };

  }
}

#endif
