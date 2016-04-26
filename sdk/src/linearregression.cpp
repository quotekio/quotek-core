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
  
    int linearRegression::train(dataset& X){

      //extracts the last columns: results
      dvector y;
      y << X.col( X.cols() -1 );
      quotek::core::utils::removeColumn(X, X.cols() -1);

      this->train(X,y);

      return 0;
    }


    int linearRegression::train(dataset& X, dvector& y) {

      cppoptlib::BfgsSolver<double> solver;
      lr_prob<double> f(X, y);
      cppoptlib::Vector<double> beta = cppoptlib::Vector<double>::Random( X.cols() );

      solver.minimize(f, beta);
      this->coefficients = beta;

    }

    int linearRegression::predict(dataset& X, std::vector<double>& y){
        
        for (int i=0;i< X.rows();i++) {

          dataset m1 = X.row(i);
          y.emplace_back( this->predict(m1));
        }

    }

    double linearRegression::predict(dataset& data){

        double result = 0;

        for (int i=0; i< data.cols(); i++ ) {
          result += data.row(0).col(i) * this->coefficients.row(i);
        }

        return result;
    }

  }
}
