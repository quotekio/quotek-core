/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#ifndef relation_HPP
#define relation_HPP

#include <string>

namespace quotek {

  namespace ml {
  
    typedef struct action {

      std::string text;
      std::string tense;
      bool negated;

    } action;

    typedef struct relation {

      std::string subject;
      quotek::ml::action action;
      std::string object;
      std::string sentence;

    } relation ;

    typedef struct sentiment {

        /** stores the computed sentiment score. */
        float score;
        
        /** Stores whether the computed sentiment is felt overall as positive or negative.
         *  If positive if false, then sentiment is persived as negative. */
        bool positive;

        /** Stores whether the sentiment is felt as mixed or not.
         *  If sentiment is mixed, you should take the result carefully.
         */
        bool mixed;

    } sentiment;


  }
}

#endif
