/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS
*/

#include "logisticregression.hpp"
using namespace Eigen;

namespace quotek {

  namespace ml {
    

  
    logisticRegression::logisticRegression() {
      this->degree = 1;
      this->regularize = false;

    }

    logisticRegression::logisticRegression(int degree) {
      this->degree = degree;
      this->regularize = false;
    }

       
    logisticRegression::logisticRegression(int degree, bool regularize) {
      this->degree = degree;
      this->regularize = regularize;
    }

    logisticRegression::~logisticRegression() {

    }
  
    int logisticRegression::train(dataset& X){

      //extracts the last columns: results
      dvector y;
      y << X.col( X.cols() -1 );
      quotek::core::utils::removeColumn(X, X.cols() -1);

      this->train(X,y);

      return 0;
    }


    int logisticRegression::train(dataset& X, dvector& y) {

      cppoptlib::BfgsSolver<double> solver;
      lor_prob<double> f(X, y);
      cppoptlib::Vector<double> beta = cppoptlib::Vector<double>::Random( X.cols() );

      solver.minimize(f, beta);
      this->coefficients = beta;

    }

    int logisticRegression::predict(dataset& X, std::vector<double>& y){
        return 0;
    }

    double logisticRegression::predict(dataset& data){
        return 0.0;
    }

  }
}
