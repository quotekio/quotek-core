/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS
*/

#ifndef ML_HPP
#define ML_HPP

#include "eigen3/Eigen/Dense"
using namespace Eigen;


namespace quotek {

  /**
   * The ml namespace contains All the functions related to Machine Learning. 
   * If you want to perform linear/logistic regressions on data, create neural nets 
   * or anything else ML-related, then the functions you need are here. */

  namespace ml {
    
    /**
     * Normalize takes a data sample and performs 0-mean and
     * unit variance transformations on it.
     */

    typedef MatrixXd dataset;

    dataset& normalize(dataset& data);


  }
}

#endif
