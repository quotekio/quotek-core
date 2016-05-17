#include <quotek/logisticregression.hpp>

#define EPSILON 0.01


void test_logistic_regression() {

  quotek::ml::dataset X;
  quotek::ml::dvector y;

  quotek::ml::dataset X_; 

  X =  MatrixXd(22,1);
  y =  VectorXd(22);

  /* This dataset will train the classifier so 
   * that any number < 10 is in class, and any number > 10 is not.
   */

  X << 10,89,112,8,32,9,12,5,23,8,56,2,3,45,6,7,15,11,13,1,999,189;
  y << 1, 0, 0, 1,0, 1,0, 1,0, 1,0, 1,1,0, 1, 1, 0, 0, 0,1,0,  0  ;

  

  quotek::ml::logisticRegression lr1;

  lr1.thereshold = 0.4;

  lr1.train(X,y);

  quotek::ml::dataset Xpred = MatrixXd(5,1);
  Xpred << 92, 8, 7, 989, 4;

  assert( lr1.predict(Xpred) == 0 );
 
  std::vector<int> mpred;

  lr1.predict(Xpred, mpred);

  assert(mpred.size() == 5);

  assert( mpred[0] == 0);
  assert( mpred[1] == 1 );
  assert( mpred[2] == 1 );
  assert( mpred[3] == 0 );
  assert( mpred[4] == 1 );
}


int main() {

  test_logistic_regression();

  exit(0);
}