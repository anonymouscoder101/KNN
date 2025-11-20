//
// Created by Shreya Bunga on 11/18/25.
//

#ifndef IMAGEMAKER_KNNIMAGECLASSIFIER_H
#define IMAGEMAKER_KNNIMAGECLASSIFIER_H

#include <vector>
#include "ImageMaker.h"

using namespace std;

class KNNImageClassifier {
private:
  struct ImageSize {
    int width;
    int height;
  };

  struct TrainImages {
    // ImageMaker img; // Object representing an image
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
  string GetLabel(string filename);
  explicit KNNImageClassifier(const string& imageDirectory, int k=3);
  double ClassificationError(vector<string> test_files);
  double EuclideanDistance(ImageMaker img1, ImageMaker img2);
  string Predict(string test_image);
};

#endif // IMAGEMAKER_KNNIMAGECLASSIFIER_H