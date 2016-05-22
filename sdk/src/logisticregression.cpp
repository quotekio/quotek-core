/*
Quotek Strategies SDK 3.0
Copyright 2013-2016 Quotek SAS
*/

#include "logisticregression.hpp"
using namespace Eigen;

namespace quotek {

  namespace ml {
    

    logisticRegression::logisticRegression() {
      this->thereshold = 0.5;
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

    int logisticRegression::predict(dataset& X, std::vector<int>& y){

      for (int i=0;i< X.rows();i++) {
          dataset m1 = X.row(i);
          y.emplace_back( this->predict(m1));
      }
    
    }

    int logisticRegression::predict(dataset& X){

      dataset m1 = X.row(0);      

      double lpred = 0;

      const cppoptlib::Vector<double> h_sigmo = 1.0/(1.0 + exp(-(m1 * this->coefficients ).array()));
      lpred = h_sigmo.sum();

      if (lpred >= this->thereshold) {
        return 1;
      }

      else {
        return 0;
      }

    }

  }
}
