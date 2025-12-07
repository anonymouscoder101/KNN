//
// Created by Shreya Bunga on 11/18/25.
//

#include "KNNImageClassifier.h"
#include <filesystem>
#include <regex>
#include <unordered_map>

/*
 * Precondition:
 * 1) img1 and img2 are fully initialized ImageMaker objects
 * 2) img1 and img2 have the same width and height
 * 3) All pixel values in both images are within valid RGB range
 *
 * Postcondition:
 * 1) Returns the sum of squared differences of corresponding RGB values between
 * img1 and img2
 * 2) The returned value is non-negative
 * 3) Neither img1 nor img2 are modified
 */
double KNNImageClassifier::EuclideanDistance(ImageMaker img1, ImageMaker img2) {
  int h = img1.height;
  int w = img1.width;
  double dist = 0.0;
  for (int row = 0; row < h; row++) {
    for (int col = 0; col < w; col++) {
      int diff = img1.image[col][row][RED] - img2.image[col][row][RED];
      dist += diff * diff;
      diff = img1.image[col][row][GREEN] - img2.image[col][row][GREEN];
      dist += diff * diff;
      diff = img1.image[col][row][BLUE] - img2.image[col][row][BLUE];
      dist += diff * diff;
    }
  }
  return dist;
}

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
 * 3) Returns the most frequently occurring label among the k nearest neighbors
 * 4) Doesn't modify the training images
 */
string KNNImageClassifier::Predict(string test_image_filename) {
  vector<DistanceWithLabel> distances;
  ImageMaker test_image = ImageMaker(test_image_filename);
  for (auto train_image : train_images) {
    double dist = EuclideanDistance(test_image, train_image.img);
    DistanceWithLabel distance_with_label = {dist, train_image.label};
    distances.push_back(distance_with_label);
  }

  // Partition the vector so that the k smallest distance objects are in the
  // beginning
  auto comparator = [](const DistanceWithLabel &lhs,
                       const DistanceWithLabel &rhs) {
    return lhs.distance < rhs.distance;
  };
  nth_element(distances.begin(), distances.begin() + k, distances.end(),
              comparator);

  // Count label within first k elements
  unordered_map<string, int> counts;
  for (int i = 0; i < k; i++) {
    counts[distances[i].label]++;
  }

  // Create a vector with labels and their frequency
  int max_count = 0;
  string max_label;
  vector<DistanceWithLabel> label_with_counts;
  for (auto &count : counts) {
    if (count.second > max_count) {
      max_count = count.second;
      max_label = count.first;
    }
  }
  return max_label;
}

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
string KNNImageClassifier::GetLabel(string filename) {
  std::string fn = std::filesystem::path(filename).filename().string();
  std::regex pattern(R"(([^_/]+)_\d+\.ppm)");
  std::smatch match;
  if (std::regex_match(fn, match, pattern)) {
    std::string label = match[1];
    return label;
  }
  return "";
}

/*
 * Precondition:
 * 1) imageDirectory exists and is a valid directory path
 * 2) The directory contains 0 or more .ppm image files
 * 3) k is a positive integer
 *
 * Postcondition:
 * 1) Initializes the classifier with k nearest neighbors
 * 2) Recursively loads all .ppm images from imageDirectory into a train_images
 * vector with all the labels parsed
 */
KNNImageClassifier::KNNImageClassifier(const string &imageDirectory, int k) {
  this->k = k;

  for (const auto &entry :
       std::filesystem::recursive_directory_iterator(imageDirectory)) {
    if (entry.is_regular_file() && entry.path().extension() == ".ppm") {
      std::string filename = entry.path().filename().string();
      TrainImages ti;
      ti.filename = entry.path().string(); // full path
      ti.label = GetLabel(filename);
      ti.img = ImageMaker(ti.filename);
      train_images.push_back(ti);
    }
  }
}

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
double KNNImageClassifier::ClassificationError(vector<string> test_files) {
  int errors = 0;
  int total = test_files.size();
  for (string &test_file : test_files) {
    string test_label = GetLabel(test_file);
    string predicted_label = Predict(test_file);
    if (predicted_label != test_label)
      errors++;
  }
  return total > 0 ? errors / static_cast<double>(total) : 0.0;
}