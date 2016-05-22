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

        dataset& add_bias(dataset& X) {

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

        //initializes centroids coordinates randomly.
        void initCentroids(dataset& X, int nb_clusters, MatrixXd& centroids) {

          centroids = MatrixXd(nb_clusters, X.cols() );

          srand(time(NULL));
          for (int i=0; i< nb_clusters;i++) {
            unsigned int rsample = quotek::core::utils::randr(0, X.rows()-1);
            VectorXd sample = X.row(rsample);
            centroids.row(i) = sample;

          }

        }

        //compute new centroids according to updated tags for each sample
        MatrixXd computeCentroids(dataset& X, VectorXd& idx, int nb_clusters ) {

          MatrixXd centroids = MatrixXd(nb_clusters, X.cols() );

          for (int i=0; i< nb_clusters; i++ ) {
 
            //nattached stores the number of points attached to old centroid.
            int nattached = 0;
            MatrixXd kvect;

            for (int j= 0; j< X.rows(); j++) {

                RowVectorXd X_j = X.row(j);

                if (idx(j) == i) {
                  nattached++;

                  
                  MatrixXd concat_(kvect.rows() + 1 , X.cols());

                  if (kvect.size() != 0) { 
                    concat_ << kvect, X_j;
                    kvect = concat_;
                  }

                  else kvect = X_j;
                  
                }
                  
            }

            //computes the updated centroid using the "barrycenter" of attached points.
            if (kvect.size() > 0) centroids.row(i) <<  kvect.colwise().sum() / nattached;


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

              MatrixXd x1 = X.row(i) - centroids.row(j);
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



      
        dvector kmeans(dataset& X, int nb_clusters) {
        

          //We declare a new matrix to store centroids.
          MatrixXd centroids;

          initCentroids(X,nb_clusters,centroids);

          //<fWe declare a new vector to store centroid attachements.
          VectorXd idx;
          //VectorXd prev_idx = VectorXd(X.rows());

          for (int i=0;i< KMEANS_MAX_ITER ; i++) {
          
            idx = findClosestCentroids(X,centroids);

            //debug
            //std::cout << "IDX:" << idx << std::endl;
            centroids = computeCentroids(X,idx,nb_clusters);

            //if ( idx == prev_idx ) break;
            //else prev_idx = idx;

          }

          return idx;

        }


        //python range(m,n) C++ impl.
        std::vector<int> range(int min, int max) {

          std::vector<int> result;

          for (int i=min;i < max;i++) {
            result.emplace_back(i);
          }
          return result;
        }

        //python range(n) C++ impl.
        std::vector<int> range(int r) {
          return range(0,r);
        }


        bool is_sorted(std::vector<int>& pool, std::vector<int>& comb) {

          int prev_idx = 0;

          for (int i=0; i< comb.size();i++) {

            int n = comb[i];

            //next we want indice of n;
            int idx = 0;

            for (int j=0;j< pool.size();j++) {
              if ( pool[j]  == n) {
                idx = j;
                break;
              }
            }

            if (idx < prev_idx ) return false;
            prev_idx = idx;

          }
          return true;
        }


        void backtrack_rec(std::vector<std::vector<int>>& result,  
                                       std::vector<int>& pool,
                                       std::vector<int> combination, 
                                       int depth, 
                                       int expected_length) {

          

          if(depth < expected_length) {

              for(int i=0 ;i<pool.size();i++) {

                  combination.push_back(pool[i]);
                  backtrack_rec(result, pool,combination, depth + 1,expected_length);
                  combination.pop_back();

              }
          }
          else if ( is_sorted(pool, combination)  ) result.emplace_back(combination);


        }

        //Combination with replacement C++ impl.
        std::vector<std::vector<int>> CWR(std::vector<int> pool, int r) {

          std::vector<std::vector<int>> result;

          std::vector<int> combination;

          backtrack_rec (result, pool, combination,0,r);
          return result;
        }


        void printvect(std::vector<int> v) {
          std::cout << "(" ;
          for (int i=0;i < v.size(); i++ ) {
            std::cout << v[i] ;
            if (i < v.size() -1) std::cout << ", ";
          }
          std::cout << ")";
        }

        dataset polynomial_features(dataset& X, int degree) {
          
          dataset result;

          //creates combinations
          std::vector<std::vector<int>> combinations;

          //if not include bias, start = 1
          for (int i= 1;i< degree + 1; i++ ) {
            
            std::vector<std::vector<int>> cbx = CWR(range( X.cols() ), i);
            combinations.insert(combinations.end(),cbx.begin(),cbx.end());

          }

          result = MatrixXd(X.rows(), combinations.size() );

          for (int i=0;i<combinations.size();i++) {
            
            MatrixXd tmp = MatrixXd( X.rows(), combinations[i].size() );

            for (int j=0;j<combinations[i].size();j++) {

              tmp.col(j) = X.col( combinations[i][j] );
            }

            result.col(i) = tmp.rowwise().prod();

          }

          return result;
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

