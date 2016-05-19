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


}


int main() {

  test_ml_add_ones();
  test_ml_pca();
  exit(0);
}