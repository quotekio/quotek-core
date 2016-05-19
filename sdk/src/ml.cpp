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


        //compute new centroids according to update tags for each sample
        MatrixXd computeCentroids(dataset& X, VectorXd& idx, int nb_clusters ) {

          MatrixXd centroids = MatrixXd(nb_clusters, X.cols() );

          for (int i=0; i< nb_clusters; i++ ) {
 
            int nattached = 0;
            VectorXd kvect;

            for (int j= 0; j< X.rows(); j++) {

                if (idx(j) == i) {
                  nattached++;
                }
                  
            }

            centroids.row(i) << (kvect.sum() / nattached);

          }        
          return centroids;
        }
        

        //tags each sample with its closest centroid index.
        VectorXd findClosestCentroids(dataset& X, MatrixXd& centroids) {
          
          VectorXd idx = VectorXd(X.rows());

          for (int i=0;i < X.rows(); i++ ) {

            double closest = 10000000000000 ;

            for (int j=0;j < centroids.rows(); j++ ) {

              /*
              R .^2: R.array().square()
              sum(R):  R.colwise().sum()
              */

              MatrixXd x1 = X.row(j) - centroids.row(j);
              MatrixXd x2 = x1.array().square();

              double dist = x2.sum();

              if (dist <= closest) {
                closest = dist;
                idx.row(i) << j;
              }

            }
          }

          return idx;
        }



      
        dataset kmeans(dataset& X, int nb_clusters) {
        
          //We declare a new matrix to store centroids.
          MatrixXd centroids = MatrixXd( nb_clusters, X.cols() );

          //We declare a new vector to store centroid attachements.
          VectorXd idx = VectorXd( X.rows() );

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

