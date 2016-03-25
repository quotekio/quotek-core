/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS
http://www.quotek.io
*/

#ifndef ALCHEMY_HPP
#define ALCHEMY_HPP

#include <string>
#include <regex>

#include "http.hpp"
#include "nlp.hpp"

namespace quotek {

  /** nlp namespace in quotek SDK regroups all the objects and methods useful to add 
   *  Langague processing functionalities fir your strategies. */
    
  namespace nlp {

    /** alchemy is a class which aims to help quotek strategies interract with Alchemy API
     *  NLP service. Alchemy API Service contains some very useful 
     *  functionalities like sentiment analysis, etc..
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
         *  for further analysis. The detected sentiment is then returned as a quotek::nlp::sentiment object.
         *  @param payload Text or URL of text to analyse.
         *  @param target tells for which target sentiment must be processed.
         *  Target is optional and therefore can be an empty string.
         *  @param format tells wether payload is text, html, or url.
         *  @return sentiment of text computed by Alchemy. */
        quotek::nlp::sentiment sentiment(std::string payload, 
                              std::string target, 
                              std::string format);

        /** relations is able to summerize a text and basically return it under a 
         *  "Who doest what" form.
         *  @param payload Text or URL of text to analyse relashionships for.
         *  @param format tells wether payload is text, html, or url.
         *  @return a vector quotek::nlp::relations
         */

        std::vector<quotek::nlp::relation> relations(std::string payload,
                                                    std::string format); 


        /** taxonomy categorizes a text corpus in a pre-existing category-tree.
         *  You can get the full category-tree at http://www.alchemyapi.com/sites/default/files/taxonomyCategories.zip
         *  @param payload Text or URL of text to get taxonomy for.
         *  @param format tells wether payload is text, html, or url.
         */

        std::vector<quotek::nlp::category> taxonomy(std::string payload,
                                                   std::string format);

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
         *  Note about API calls: You can find the whole list of Alchemy API Calls and full documentation here: <a href="http://www.alchemyapi.com/api">http://www.alchemyapi.com/api</a>
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
