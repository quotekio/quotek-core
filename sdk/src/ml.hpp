/*
Quotek Strategies SDK 2.3
Copyright 2013-2016 Quotek SAS
*/

#ifndef ML_HPP
#define ML_HPP

#include <iostream>

#include "eigen3/Eigen/Dense"
using namespace Eigen;


namespace quotek {

  /**
   * The ml namespace contains All the functions related to Machine Learning. 
   * If you want to perform linear/logistic regressions on data, create neural nets 
   * or anything else ML-related, then the functions you need are here. */

  namespace ml {
    
    
    typedef MatrixXd dataset;

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
     * @param X: dataset to labelize
     * @param nb_clusters: integer which defines the number of categories wanted for clustering.
     * @return Labelized dataset: a new column which contains values in [1,nb_clusters] for each sample, is created.
     */
     
    dataset kmeans(dataset& X, int nb_clusters);

  }
}

#endif
