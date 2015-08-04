/*
Quotek Strategies SDK 2.0
Copyright 2013-2015 Quotek SAS
http://www.quotek.io
*/

#ifndef ALCHEMY_HPP
#define ALCHEMY_HPP

#include <string>
#include <regex>

#include "http.hpp"
#include "sentiment.hpp"

namespace quotek {

  /** ml namespace in quotek SDK regroups all the objects and methods useful to implement 
   *  Machine Learning/Deep Learning Algorithms in your strategies. */
  namespace ml {

    /** alchemy is a class which aims to help quotek strategies interract with Alchemy API
     *  Machine-Larning service. Alchemy API Service contains some very useful 
     *  ML functionalities like sentiment analysis, etc..
     *
     *  Note: Only the API calls which are most useful to algo-trading are added here.
     */
    class alchemy {

      public:

        /** alchemy class constructor.
         *  @param key Alchemy API Key.
         *  @param host Alchemy host endpoint. 
         */
        alchemy(std::string key, std::string endpoint);

        /** alchemy object destructor. */
        ~alchemy();
        
        /** Sentiment Analysis method. The payload text/url passed as argument is sent to the API
         *  for further analysis. The detected sentiment is then returned as a quotek::ml::sentiment object.
         *  @param payload Text or URL of text to analyse.
         *  @param target tells for which target sentiment must be processed.
         *  Target is optional and therefore can be an empty string.
         *  @param format tells wether payload is text, html, or url.
         *  @return sentiment of text computed by Alchemy. */
        quotek::ml::sentiment sentiment(std::string payload, 
                              std::string target, 
                              std::string format);


        

        /** relations is able to summerize a text and basically return it under a "Who doest what" form.
        */
        /*quotek::ml::relations relations(std::string payload, 
                              std::string target, 
                              std::string format);*/      

        /** Ranked concepts is a function that is able to deduce the different concepts developped
         *  in the provided text and to create relationships between them. 
         *  @param payload Text or URL of text to analyse.
         *  @param format tells wether payload is text, html, or url.
         *  @return the map of concepts computed by Alchemy. */

        /*quotek::ml::concepts ranked_concepts(std::string payload,
                                             std::string format);*/

        /** language function guesses the language used to write the text passed as argument.
         *  @param payload Text or URL of text to analyse.
         *  @param format tells wether payload is text, html, or url.
         *  @return Detect Language string.
         */
        std::string language(std::string payload,
                             std::string format);

        std::string key;
        std::string endpoint;

      private:
        std::string query(std::string fct, std::string payload, std::string format);

    };
    


  }

}


#endif
