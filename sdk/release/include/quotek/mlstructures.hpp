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
  

    /** action struct is meant to store detected action for a sentence. Action is composed of 3 components,
     text (verb) , tense, and negated (if verb is used in its negative form). */
    typedef struct action {

      /** stores the verb. */
      std::string text;

      /** stores the verb tense. */
      std::string tense;

      /** tells if verb is used in negative form. */
      bool negated;

    } action;

    /** Relation structures contain results of machine learning relationship algorithms, which
      * mainly allow to guess "Who does what". This is a trivial form of summerizing which allows 
      * machine processing of complex texts. 
      */

    typedef struct relation {

      /** Tells who/what is the subject of the sentence. */
      std::string subject;
      /** Tells what and kind of action is performed in sentence. */
      quotek::ml::action action;
      /** Tells on what/who the action is performed. */
      std::string object;

      /** Initial sentence, extracted from text. */
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
