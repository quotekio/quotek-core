#include <quotek/ml.hpp>

#define EPSILON 0.01


void test_ml_add_ones() {

  quotek::ml::dataset X;

  X =  MatrixXd(5,2);
  X << 4,4,4,4,4,4,4,4,4,4;

  quotek::ml::add_ones(X);
  
  assert( X.cols() == 3  );

  VectorXd v1 = X.col(0);

  for (int i=0; i< v1.rows(); i++ ) {
    assert(v1.row(i)[0] == 1);
  }
  
}


void test_ml_pca() {

  quotek::ml::dataset X;
  X = MatrixXd(4,3);

  X << 2,3,1,4,9,1,6,8,1,1,12,1;

  quotek::ml::dataset Xpca = quotek::ml::pca(X,1);

  assert( Xpca.cols() == 1  );
  assert(Xpca.col(0)[0] > 2.5 && Xpca.col(0)[0] < 3.5  );
  assert(Xpca.col(0)[1] > 8.5 && Xpca.col(0)[1] < 9.5  );
  assert(Xpca.col(0)[2] > 7.5 && Xpca.col(0)[2] < 8.5  );

  quotek::ml::dataset Xpca2 = quotek::ml::pca(X,2);

  assert( Xpca2.cols() == 2  );
  assert(Xpca2.col(1)[0] > 1.5 && Xpca2.col(1)[0] < 2.5  );
  assert(Xpca2.col(1)[1] > 3.2 && Xpca2.col(1)[1] < 4.8  );
  assert(Xpca2.col(1)[2] > 5.2 && Xpca2.col(1)[2] < 6.8  );


}


void test_ml_kmeans() {

}

void test_ml_polyfeat() {

}


int main() {

  test_ml_add_ones();
  test_ml_pca();
  test_ml_kmeans();
  test_ml_polyfeats();
  exit(0);
}