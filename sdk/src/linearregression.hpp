/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS
*/

#ifndef LREG_HPP
#define LREG_HPP

#include <iostream>
#include "ml.hpp"
#include "matrixutils.hpp"

namespace quotek {

  namespace ml {
    
    /**
     * The linearRegressions class allows to perform linear regression learning 
     * algorithms on some datasets. */

    class linearRegression {

      public:

        /** Class simplest constructor */
        linearRegression();

         /** Class constructor 2
         *  @param regularize Tells if features must be regularized to avoid overfitting.
         */

        linearRegression(bool regularize);

        /** Object Destructor */
        ~linearRegression();

        /** train takes a dataset and creates a fitting model according to the provided data.
         *  Note: it assusmes that the last column of the dataset stores the expected results (y).
         *  @param X Dataset to modelize.  
         */
         int train(dataset& X);

        /** Same than regular train() except that it assumes the expected results are in a splitted,
         *  m-dimensions vector.
         *  @param X Dataset to modelize.
         *  @param y results vector.
         */
        int train(dataset& X, dvector& y);

        /** predict takes a small dataset and try to guess the output according to the 
         *  previously learned model.
         *  @param X Data to predict outputs for.
         *  @param y Predicted outputs, stored as a vector of floats.
         */

        int predict(dataset& data, std::vector<double>& y);

         /** predict takes a small dataset and try to guess the output according to the 
         *  previously learned model.
         *  @param X Data to predict output for (array must have 1 single line).
         *  @return predicted output, as a float.
         */

        double predict(dataset& X);

        /** stores wether we will use regularization or not. */
        bool regularize;

        /** stores the coefficients for each dimension of the dataset. */
        VectorXd coefficients;

    };

     template<typename T>
     class lr_prob : public cppoptlib::Problem<T> {
        const cppoptlib::Matrix<T> X;
        const cppoptlib::Vector<T> y;
        const cppoptlib::Matrix<T> XX;

      public:
        lr_prob(const cppoptlib::Matrix<T> &X_, const cppoptlib::Vector<T> y_) : X(X_), y(y_), XX(X_.transpose()*X_) {}

        T value(const cppoptlib::Vector<T> &beta) {
            return 0.5*(X*beta-y).squaredNorm();
        }

        void gradient(const cppoptlib::Vector<T> &beta, cppoptlib::Vector<T> &grad) {
            grad = XX*beta - X.transpose()*y;
        }
     };

  }
}

#endif
