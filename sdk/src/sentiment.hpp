/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
*/

#ifndef SENTIMENT_HPP
#define SENTIMENT_HPP

#include <string>

namespace quotek {

  namespace ml {
  
    /** sentiment is a class meant to store sentiment analysis results */
    class sentiment {

      public:
      	
        /** sentiment constructor 1 */
        sentiment();

        /** sentiment constructor 2 */
        sentiment(float score, bool positive, bool mixed);

        ~sentiment();

        /** stores the computed sentiment score. */
      	float score;
      	
      	/** Stores whether the computed sentiment is felt overall as positive or negative.
      	 *  If positive if false, then sentiment is persived as negative. */
      	bool positive;

        /** Stores whether the sentiment is felt as mixed or not.
         *  If sentiment is mixed, you should take the result carefully.
         */
      	bool mixed;
    };

  }
}

#endif
