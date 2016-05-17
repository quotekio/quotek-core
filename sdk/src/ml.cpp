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

        dataset pca(dataset& X, int feats) {

          dataset result;

          MatrixXd centered = X.rowwise() - X.colwise().mean();
          MatrixXd cov = centered.adjoint() * centered;

          SelfAdjointEigenSolver<MatrixXd> eig(cov);

          std::cout << eig.eigenvectors().rightCols(feats) << std::endl;

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

