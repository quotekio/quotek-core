#include <quotek/linearregression.hpp>

void test_linear_regression() {

  quotek::ml::dataset X;
  quotek::ml::dvector y;

  quotek::ml::dataset X_; 

  X =  MatrixXd(1, 5);
  y =  VectorXd(5);
  
  X << 1 , 2, 3, 4, 5;
  y << 1,  2, 3, 4, 5;

  quotek::ml::linearRegression l1;

  l1.train(X,y);


  //l1.predict()
  //assert(    );



}


int main() {

  test_linear_regression();
}