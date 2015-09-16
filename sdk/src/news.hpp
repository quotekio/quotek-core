/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#ifndef NEWS_HPP
#define NEWS_HPP

namespace quotek {

  namespace data {

    /**
     * news is a container which aims to store data comming from remote financial news feeds, being RSS, twitter, etc..
     */
    class news {

      public:
         
       /** news constructor */
       news() {

       }

       /** news destructor */
       ~news () {

       }

       /** Stores the timestamp on which the news was published. */
       long date;

       /** Stores the source/author of the news. */
       std::string source;

       /** Stores the title of the news (if appliable). */
       std::string title;
 
       /** Stores the main content of the news. */
       std::string content;

       /** Stores the related tags of the news (if appliable). */
       std::vector<std::string> tags;

    };

  }

}

#endif