/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS
*/

#ifndef LREG_HPP
#define LREG_HPP

#include <iostream>

#include "ml.hpp"
using namespace Eigen;

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
         *  @param degree: Number of degrees of the expected model curve.
         */

        linearRegression(int degree);

         /** Class constructor 3
         *  @param degree Number of degrees of the expected model curve.
         *  @param regularize Tells if polynomial model must be regularized to avoid overfitting.
         */
         
        linearRegression(int degree, bool regularize);

        /** Object Destructor */
        ~linearRegression();

        /** train takes a dataset and creates a fitting model according to the provided data.
         *  @param data Dataset to modelize. 
         */
        int train(dataset& data);

        /** predict takes a small dataset and try to guess the output according to the 
         *  previously learned model.
         *  @param dataset Data to predict outputs for.
         *  @param results Predicted outputs, stored as a vector of floats.
         */

        int predict(dataset& data, std::vector<float>& results);

         /** predict takes a small dataset and try to guess the output according to the 
         *  previously learned model.
         *  @param dataset Data to predict output for (array must have 1 single line).
         *  @return predicted output, as a float.
         */

        float predict(dataset& data);

        /** stores the polynomial degree of the model to fit. */
        int degree;

        /** stores wether we will use regularization or not. */
        bool regularize;

        /** stores the coefficients for each dimension of the dataset. */
        std::vector<float> coefficients;

    };

  }
}

#endif
