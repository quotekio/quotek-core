#ifndef ALCHEMY_HPP
#define ALCHEMY_HPP

#include <string>
#include "http.hpp"

namespace quotek {


  /** ml namespace in quotek SDK regroups all the objects and methods useful to implement 
   *  Machine Learning/Deep Learning Algorithms in your strategies */
  namespace ml {

    /** alchemy is a class which aims to help quotek strategies interract with Alchemy API
     *  Machine-Larning service. Alchemy API Service contains some very useful 
     *  ML functionalities like sentiment analysis, etc..
     */
    class alchemy {

      public:

        /** alchemy class constructor.
         *  @param key Alchemy API Key.
         *  @param host Alchemy host endpoint. 
         */
        alchemy(std::string key, std::string host);

        /** alchemy object destructor. */
        ~alchemy();
        
        /** Sentiment Analysis method. The payload text/url passed as argument is sent to the API
         *  for further analysis. The detected sentiment is then returned as a string.
         *  @param payload Text or URL of text to analyse.
         *  @param target tells for which target sentiment must be processed.
         *  Target is optional and therefore can be an empty string.
         *  @param format tells wether payload is raw text or HTML-formated.
         *  @return sentiment of text computed by Alchemy. */
        std::string sentiment(std::string payload, std::string target, std:string format);


        std::string key;
        std::string host;

        
    };
    


  }

}


#endif
