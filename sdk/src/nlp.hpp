/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS
http://www.quotek.io
*/

#ifndef NLP_HPP
#define NLP_HPP

#include <string>

namespace quotek {

  namespace nlp {
  
    
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
      quotek::nlp::action action;
      /** Tells on what/who the action is performed. */
      std::string object;

      /** Initial sentence, extracted from text. */
      std::string sentence;

    } relation ;

    typedef struct sentiment {

        /** stores the computed sentiment score. */
        float score;
        
        /** Stores whether the computed sentiment is felt overall as positive, negative or neutral.
         *  so data stored in here can be "positive", "negative" or "neutral"  */
        std::string sentimentstr;

        /** Stores whether the sentiment is felt as mixed or not.
         *  If sentiment is mixed, you should take the result carefully.
         */
        bool mixed;

        /** Tells if sentiment analysis failed or not */
        bool error;

        /** Stores the analysis error message in case of failure */
        std::string error_message;

    } sentiment;
    

    /** taxonomy is a structure created to store text categorization */
    typedef struct category {

      /** Stores the data detected category as a string */
      std::string name;

      /** Stores the categorization score (level of confidence) */
      float score;

      /** Level of confidence summurizing. True if confident, false otherwise. */
      bool confident;

    } category;


  }
}

#endif
