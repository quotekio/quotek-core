/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS
*/

#ifndef LOREG_HPP
#define LOREG_HPP

#include <iostream>
#include "ml.hpp"
#include "matrixutils.hpp"

namespace quotek {

  namespace ml {
    
    /**
     * The logisticRegression class allows to perform logistic regression learning 
     * algorithms on some datasets. */

    class logisticRegression {

      public:

        /** Class simplest constructor */
        logisticRegression();

        /** Object Destructor */
        ~logisticRegression();

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

        int predict(dataset& X, std::vector<int>& y);

         /** predict takes a small dataset and try to guess the output according to the 
         *  previously learned model.
         *  @param X Data to predict output for (array must have 1 single line).
         *  @return predicted output, as a float.
         */

        int predict(dataset& X);

        /** stores wether we will use regularization or not. */
        bool regularize;

        /** thereshold is the number in [0,1] that will make that an element will be 
         *  labeled as being part of a class, or not.
         *  Default value is 0.5.
         */
        double thereshold;

        /** stores the coefficients for each dimension of the dataset. */
        VectorXd coefficients;

    };

     template<typename T>
     class lor_prob : public cppoptlib::Problem<T> {
        const cppoptlib::Matrix<T> X;
        const cppoptlib::Vector<T> y;
        const cppoptlib::Matrix<T> XX;

      public:
        
        lor_prob(const cppoptlib::Matrix<T> &X_, const cppoptlib::Vector<T> y_) : X(X_), y(y_), XX(X_.transpose()*X_) {}

        T value(const cppoptlib::Vector<T> &beta) {

                return ( 1.0/(1.0 + exp(-(X*beta).array())) - y.array()).matrix().squaredNorm();
            }

        void gradient(const cppoptlib::Vector<T> &beta, cppoptlib::Vector<T> &grad) {
                
                const cppoptlib::Vector<T> h_sigmo = 1.0/(1.0 + exp(-(X*beta).array()));

                grad = X.transpose()*(h_sigmo - y);
        }

        
     };

  }
}

#endif
