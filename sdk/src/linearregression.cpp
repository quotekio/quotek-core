/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS
*/

#include "linearregression.hpp"

using namespace Eigen;

namespace quotek {

  namespace ml {
    
  
    linearRegression::linearRegression() {
      this->degree = 1;
      this->regularize = false;

    }

    linearRegression::linearRegression(int degree) {
      this->degree = degree;
      this->regularize = false;
    }

       
    linearRegression::linearRegression(int degree, bool regularize) {
      this->degree = degree;
      this->regularize = regularize;
    }

    linearRegression::~linearRegression() {

    }
  
    int linearRegression::train(dataset& data){
      return 0;
    }
    int linearRegression::predict(dataset& data, std::vector<float>& results){
        return 0;
    }

    float linearRegression::predict(dataset& data){
        return 0.0;
    }

  }
}
