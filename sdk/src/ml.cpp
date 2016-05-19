/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS
*/

#include "ml.hpp"

namespace quotek {

  namespace ml {
    
        dataset& normalize(dataset& X) {

          RowVectorXd mean = X.colwise().mean();
          RowVectorXd std = ((X.rowwise() - mean).array().square().colwise().sum() / (X.rows() - 1)).sqrt();
          
          X = (X.rowwise() - mean).array().rowwise() / std.array();

          return X;

        }

        dataset& add_ones(dataset& X) {

          X.conservativeResize(X.rows(),X.cols() + 1);

          //shifts data to the last column
          for (int i= X.cols() -2; i >= 0; i-- ) {
            X.col(i+1) = X.col(i);
          }

          VectorXd v1 =  VectorXd( X.rows() );
          for (int i=0;i< X.rows(); i++ ) {
            v1.row(i) << 1;
          }

          X.col(0) = v1;
        
          return X;
        }

        dataset pca(dataset& X, int feats) {

          dataset result;

          MatrixXd aligned = X.rowwise() - X.colwise().mean();
          JacobiSVD<MatrixXd> svd(aligned, ComputeThinV);

          MatrixXd W = svd.matrixV().leftCols(feats);

          result = X * W;
          return result;
        }


        dataset kmeans(dataset& X, int nb_clusters) {

        }


        dataset& polynomial_features(dataset& X, int degree) {
          
          int prev_cols = X.cols();
          int nnc = X.cols() + (degree -1 ) * X.cols();
          X.conservativeResize(X.rows(), nnc);

          
          return X;
        }

        double nl_sigmoid(double input) {
          return (1.0 / ( 1.0 + exp (-1* input) ) );
        }

        dataset nl_sigmoid(dataset& input) {

          dataset result;
          //result = input.unaryExpr( [](double x) { return nl_sigmoid(x); });
          result = input.unaryExpr( static_cast<double (*)(double input)>(&nl_sigmoid) );
          return result;

        }

        double nl_rectifier(double input) {
          return ( input > 0.0 ) ? input : 0.0 ;
        }

        dataset nl_rectifier(dataset& input) {

          dataset result;
          //result = input.unaryExpr([](double x) { return nl_rectifier(x); });
          result = input.unaryExpr( static_cast<double (*)(double input)>(&nl_rectifier) );
          return result;
        }

        dataset nl_tanh(dataset& input) {
          dataset result;
          result = input.unaryExpr([](double x) { return tanh(x); });
          return result;
        }

  }
}

