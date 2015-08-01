/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#include "sentiment.hpp"

namespace quotek {

  namespace ml {
     
        sentiment::sentiment() {

        }
        
        sentiment::sentiment(float score, bool positive, bool mixed) {
          this->score = score;
          this->positive = positive;
          this->mixed = mixed;
        }

        sentiment::~sentiment() {

        }       
  }
}
