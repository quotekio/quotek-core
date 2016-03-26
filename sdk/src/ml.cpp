/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS
*/

#include "ml.hpp"

namespace quotek {

  namespace ml {
    
        dataset& normalize(dataset& data) {

          data = data - data.colwise().mean(); 

          return data;

        }

  }
}

