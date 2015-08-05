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
#include "mlstructures.hpp"

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

        /** relations is able to summerize a text and basically return it under a 
         *  "Who doest what" form.
         *  @param payload Text or URL of text to analyse relashionships for.
         *  @param format tells wether payload is text, html, or url.
         *  @return a vector a relations. under the form of an quotek::core:
         */

        std::vector<quotek::ml::relation> relations(std::string payload,
                                                    std::string format); 

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

        /** Since alchemy binding is only partially supported, raw() allows to perform an API call 
         *  by letting the user choose which call to use and also which parameters to pass it.
         *  It will then return the Alchemy server response in raw form (no parsing).
         *  @param call: name of the Alchemy API call (without URI format prefix).
         *  @param parameters: POST parameters to send with the call.
         *  @return ALchemy server response as raw text.
         *
         *  Note about API calls: You can find the whole list of Alchemy API Calls and full documentation here: http://www.alchemyapi.com/api
         *  Note about call parameters: You are not required to add apikey in the parameters list, 
         *  since raw() will append it for you.
         */
        std::string raw(std::string call,
                        std::map<std::string, std::string> parameters);


        std::string key;
        std::string endpoint;

      private:
        std::string query(std::string fct, std::string payload, std::string format);

    };
    


  }

}


#endif
