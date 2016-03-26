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

          

          return result;
        }

  }
}

