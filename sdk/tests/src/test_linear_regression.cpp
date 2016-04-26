#include <quotek/linearregression.hpp>

#define EPSILON 0.01


void test_linear_regression() {

  quotek::ml::dataset X;
  quotek::ml::dvector y;

  quotek::ml::dataset X_; 

  X =  MatrixXd(10,1);
  y =  VectorXd(10);

  X << 1,2,3,4,5,6,7,8,9,10;
  y << 2,4,6,8,10,12,14,16,18,20;

  quotek::ml::linearRegression l1;

  l1.train(X,y);

  double c1 = l1.coefficients.row(0)[0]; 

  assert( fabs( c1 - 2.0) < EPSILON );

  quotek::ml::dataset Xpred = MatrixXd(3,1);
  Xpred << 150, 200, 500;

  double pred = l1.predict(Xpred);
  
  assert( fabs( pred - 300) < EPSILON );

  std::cout << "Testing multi-predict.." << std::endl;

  std::vector<double> mpred;

  l1.predict(Xpred, mpred);
  
  assert(mpred.size() == 3);

  assert( fabs( mpred[1] - 400) < EPSILON );
  assert( fabs( mpred[2] - 1000) < EPSILON );

}


int main() {

  test_linear_regression();
}