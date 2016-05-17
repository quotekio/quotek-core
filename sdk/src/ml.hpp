/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS
*/

#ifndef ML_HPP
#define ML_HPP

#include <iostream>

#include "eigen3/Eigen/Dense"
#include "cppoptlib/meta.h"
#include "cppoptlib/problem.h"
#include "cppoptlib/solver/bfgssolver.h"

using namespace Eigen;


namespace quotek {

  /**
   * The ml namespace contains All the functions related to Machine Learning. 
   * If you want to perform linear/logistic regressions on data, create neural nets 
   * or anything else ML-related, then the functions you need are here. */

  namespace ml {
    
    
    typedef MatrixXd dataset;
    typedef VectorXd dvector;

    /**
     * normalize takes a data sample and performs 0-mean and
     * unit variance transformations on it.
     * @param X dataset to normalize.
     * @return reference to transformed dataset (X)
     */
    dataset& normalize(dataset& X);

    /**
     * pca performs Principal Component Analysis on 
     * a given n-dimensions dataset.
     * @param X: dataset to perform PCA on.
     * @param feats: Number of features to keep after PCA reduction. 
     * Note: feats must be between 1 and columns(X) - 1
     * @return PCA-reduced dataset.
     */
    dataset pca(dataset& X, int feats);


    /**
     * kmeans performs a K-means clustering algorithm
     * on a given dataset to in order to labelize its samples. 
     * This algorithm works very nicelly when you don't have pre-labelled data and don't
     * want to labelize them manually.
     * @param X dataset to labelize
     * @param nb_clusters integer which defines the number of categories wanted for clustering.
     * @return Labelized dataset: a new column which contains values in [1,nb_clusters] for each sample, is created.
     */
     
    dataset kmeans(dataset& X, int nb_clusters);


    /**
     * polynomial_features is a function that takes a dataset to 
     * increase its number of dimensions with polynomial
     * elements: for instance [a,b] features become at degree 2: [1, a, b, ab, a^2, b^2 ].
     * @param X dataset create poly features for.
     * @param degree Number of dimensions to add.
     * @return dimentionality improved dataset
     */
     dataset& polynomial_features(dataset& X, int degree);

    /**
     * computes the sigmoid function of a single double value.
     * @param input input to compute sigmoid for.
     * @return the sigmoid value for input.
     */

    double nl_sigmoid(double input) ;

    /** vectorized version of nl_sigmoid. */
    dataset nl_sigmoid(dataset& input);

    /**
     * nl_rectifier, or Rectified Linear Unit, computes max(0,input) for a single value.
     * @param input input to rectify.
     * @return the rectified value for input.
     */
    double nl_rectifier(double input);

     /** vectorized version of nl_rectifier. */
    dataset nl_rectifier(dataset& input);

    /** vectorized implementation of tanh. */
    dataset nl_tanh(dataset& input);

  }
}

#endif
