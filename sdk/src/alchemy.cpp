#include "alchemy.hpp"
#include <rapidjson/document.h>


namespace quotek {

  namespace ml {


        alchemy::alchemy(std::string key, std::string host) {
          this->key = key;
          this->host = key;
        }
        alchemy::~alchemy() {

        }

        std::string alchemy::sentiment(std::string payload, 
                                       std::string target, 
                                       std:string format) {

          if (format == "") {

            if ( startswith(payload,"http://") || startswith(payload,"https://") ) {

            }

          } 

          else {

            if ( startswith(payload,"http://") || startswith(payload,"https://") ) {

            }

          }

        }
        
    }
}
