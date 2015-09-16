/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#ifndef DATASOURCE_HPP
#define DATASOURCE_HPP

#include <string>
#include "record.hpp"

namespace quotek {

  namespace datasource {
     
    /**
     * datasource is a meta class meant to retrieve data from multiple data sources.
     */ 
    class datasource {

      public:

        /** Tells where is the location of the datasource.
         *  It must have the following template: "scheme://[username:password]@location".
         *  supported schemes are:
         *    - http:// : for remote http content.
         *    - s3://   : for fetching content from an S3 bucket.
         *    - sql://  : fetching remote database content.
         *    - file:// : for local data files.
         */

      	std::string source;

      	datasource() {}
      	~datasource() {}

    };

  }
}

#endif
