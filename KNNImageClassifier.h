//
// Created by Shreya Bunga on 11/18/25.
//

#ifndef IMAGEMAKER_KNNIMAGECLASSIFIER_H
#define IMAGEMAKER_KNNIMAGECLASSIFIER_H

#include "ImageMaker.h"
#include <vector>

using namespace std;

class KNNImageClassifier {
private:
  struct ImageSize {
    int width;
    int height;
  };

  struct TrainImages {
    ImageMaker img; // Object representing an image
    string filename;
    string label; // Image category
  };

  struct DistanceWithLabel {
    double distance;
    string label;
  };

  int k;
  vector<TrainImages> train_images;

public:
  /*
   * Precondition:
   * 1) filename is a valid string representing a file path or file name
   * 2) The filename follows the format <label>_<number>.ppm
   *
   * Postcondition:
   * 1) Extracts and returns the label portion of the filename
   * 2) If the filename doesn't match the expected pattern, returns an empty
   * string
   */
  string GetLabel(string filename);

  /*
   * Precondition:
   * 1) imageDirectory exists and is a valid directory path
   * 2) The directory contains 0 or more .ppm image files
   * 3) k is a positive integer
   *
   * Postcondition:
   * 1) Initializes the classifier with k nearest neighbors
   * 2) Recursively loads all .ppm images from imageDirectory into a
   * train_images vector with all the labels parsed
   */
  explicit KNNImageClassifier(const string &imageDirectory, int k = 3);

  /*
   * Precondition:
   * 1) test_files contains valid file paths to PPM image files
   * 2) Each test file name follows the expected name format
   * 3) The classifier has already been initialized with training images
   * 4) All test images have the same dimensions as the training images
   *
   * Postcondition:
   * 1) Predicts the label for each test image
   * 2) Compares each predicted label with the true label
   * 3) Returns the classification error as a value between 0.0 and 1.0
   * 4) If test_files is empty, returns 0.0
   * 5) Doesn't modify the training images
   */
  double ClassificationError(vector<string> test_files);

  /*
   * Precondition:
   * 1) img1 and img2 are fully initialized ImageMaker objects
   * 2) img1 and img2 have the same width and height
   * 3) All pixel values in both images are within valid RGB range
   *
   * Postcondition:
   * 1) Returns the sum of squared differences of corresponding RGB values
   * between img1 and img2
   * 2) The returned value is non-negative
   * 3) Neither img1 nor img2 are modified
   */
  double EuclideanDistance(ImageMaker img1, ImageMaker img2);

  /*
   * Precondition:
   * 1) test_image_filename refers to a valid PPM image file
   * 2) train_images vector has been initialized and contains at least k images
   * 3) All training images have the same dimensions as the test image
   * 4) k is a positive integer and k <= train_images.size()
   *
   * Postcondition:
   * 1) Computes the Euclidean distance between the test image and each training
   * image
   * 2) Determines the k nearest neighbors based on smallest distance values
   * 3) Returns the most frequently occurring label among the k nearest
   * neighbors
   * 4) Doesn't modify the training images
   */
  string Predict(string test_image);
};

#endif // IMAGEMAKER_KNNIMAGECLASSIFIER_H