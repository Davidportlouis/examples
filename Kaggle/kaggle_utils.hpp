/**
 * Utitlity functions that is useful for solving Kaggle problems
 * 
 * @author Eugene Freyman
 */
 
#ifndef MODELS_KAGGLE_UTILS_HPP
#define MODELS_KAGGLE_UTILS_HPP

#include <iostream>
#include <fstream>

#include <armadillo>

/**
 * Returns labels bases on predicted probability (or log of probability)  
 * of classes.
 * @param predOut matrix contains probabilities (or log of probability) of
 * classes. Each row corresponds to a certain class, each column corresponds
 * to a data point.
 * @return a row vector of data point's classes. The classes starts from 1 to
 * the number of rows in input matrix.
 */
arma::Row<int> getLabels(const arma::mat& predOut) 
{
  arma::Row<int> pred = arma::zeros<arma::Row<int>>(predOut.n_cols);
  
  // Class of a j-th data point is chosen to be the one with maximum value
  // in j-th column plus 1 (since column's elements are numbered from 0).
  for (unsigned int j = 0; j < predOut.n_cols; ++j)
  {
    pred(j) = arma::as_scalar(arma::find(
        arma::max(predOut.col(j)) == predOut.col(j), 1)) + 1;
  }
  
  return pred;
}

/**
 * Returns the accuracy (percentage of correct answers).
 * @param predLabels predicted labels of data points.
 * @param realY real labels (they are double because we usually read them from
 * CSV file that contain many other double values).
 * @return percentage of correct answers.
 */
double accuracy(arma::Row<int> predLabels, const arma::mat& realY)
{
  // Calculating how many predicted classes are coincide with real labels.
  int success = 0;
  for (unsigned int j = 0; j < realY.n_cols; j++) {
    if (predLabels(j) == std::round(realY(j))) {
      ++success;
    }  
  }
  
  // Calculating percentage of correctly classified data points.
  return (double)success / (double)realY.n_cols * 100.0;
}

/**
 * Saves prediction into specifically formated CSV file, suitable for 
 * the most Kaggle competitions.
 * @param filename the name of a file.
 * @param header the header in a CSV file.
 * @param predLabels predicted labels of data points. Classes of data points
 * are expected to start from 1. At the same time classes of data points in
 * the file are going to start from 0 (as Kaggle usually expects)
 */
void save(const std::string filename, std::string header, 
  const arma::Row<int>& predLabels)
{
	std::ofstream out(filename);
	out << header << std::endl;
	for (unsigned int j = 0; j < predLabels.n_cols; ++j)
	{
	  // j + 1 because Kaggle indexes start from 1
	  // pred - 1 because 1st class is 0, 2nd class is 1 and etc.
		out << j + 1 << "," << std::round(predLabels(j)) - 1;
    // to avoid an empty line in the end of the file
		if (j < predLabels.n_cols - 1)
		{
		  out << std::endl;
		}
	}
	out.close();
}

#endif