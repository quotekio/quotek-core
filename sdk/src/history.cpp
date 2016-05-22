/*
Quotek Strategies SDK 3.0
Copyright 2013-2016 Quotek SAS
http://www.quotek.io
*/

#include "history.hpp"

namespace quotek {

  namespace data {

    history::history() {

    }

    history::history(long timestamp, 
                     float open, 
                     float close,
                     float high,
                     float low) {

      this->timestamp = timestamp;
      this->open = open;
      this->close = close;
      this->high = high;
      this->low = low;
    }
      
    history::~history () {
    
    }

  }
}
