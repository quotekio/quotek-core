#include <quotek/ml.hpp>

#define EPSILON 0.01


void test_ml_add_bias() {

  quotek::ml::dataset X;

  X =  MatrixXd(5,2);
  X << 4,4,4,4,4,4,4,4,4,4;

  quotek::ml::add_bias(X);
  
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

  quotek::ml::dataset X =  MatrixXd(10,2);
  X << 1,1,2,2,3,3,4,4,5,5,116,119,110,115,132,137,129,118,116,119;

  quotek::ml::dvector v1 = quotek::ml::kmeans(X,2);

  assert(  v1.row(0)  == v1.row(1));
  assert(  v1.row(0)  == v1.row(2)); 
  assert(  v1.row(0)  == v1.row(3));
  assert(  v1.row(0)  == v1.row(4));  

  assert(  v1.row(5)  == v1.row(6));
  assert(  v1.row(5)  == v1.row(7)); 
  assert(  v1.row(5)  == v1.row(8));
  assert(  v1.row(5)  == v1.row(9));  


}

void test_ml_polyfeats() {
  
  quotek::ml::dataset X = MatrixXd(3,2);
  X << 0,1,2,3,4,5;

  quotek::ml::dataset Xres = MatrixXd(3,5);
  Xres << 0,1,0,0,1,2,3,4,6,9,4,5,16,20,25;

  quotek::ml::dataset X2 = quotek::ml::polynomial_features(X,2);

  assert( X2 == Xres ); 

}


int main() {

  test_ml_add_bias();
  test_ml_pca();
  test_ml_kmeans();
  test_ml_polyfeats();
  exit(0);
}