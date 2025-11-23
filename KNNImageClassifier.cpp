//
// Created by Shreya Bunga on 11/18/25.
//

#include "KNNImageClassifier.h"
#include <unordered_map>
#include <filesystem>
#include <regex>

double KNNImageClassifier::EuclideanDistance(ImageMaker img1, ImageMaker img2) {
  int h = img1.GetHeight();
  int w = img1.GetWidth();
  double dist = 0.0;
  for (int row = 0; row < h; row++) {
    for (int col = 0; col < w; col++) {
      double diff = img1.GetPixel(row, col, RED) - img2.GetPixel(row, col, RED);
      dist += diff * diff;
      diff = img1.GetPixel(row, col, GREEN) - img2.GetPixel(row, col, GREEN);
      dist += diff * diff;
      diff = img1.GetPixel(row, col, BLUE) - img2.GetPixel(row, col, BLUE);
      dist += diff * diff;
    }
  }
  return dist;
}

string KNNImageClassifier::Predict(string test_image_filename) {
  vector<DistanceWithLabel> distances;
  ImageMaker test_image = ImageMaker(test_image_filename);
  for (auto train_image : train_images) {
    double dist =
        EuclideanDistance(test_image, ImageMaker(train_image.filename));
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
  for (int i = 0; i < k; i++){
    counts[distances[i].label]++;
  }

  // Create a vector with labels and their frequency
  int max_count = 0;
  string max_label;
  vector<DistanceWithLabel> label_with_counts;
  for (auto &count: counts) {
    if (count.second > max_count) {
      max_count = count.second;
      max_label = count.first;
    }
  }
  return max_label;
}

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

KNNImageClassifier::KNNImageClassifier(const string &imageDirectory, int k) {
  this->k = k;

  for (const auto &entry :
       std::filesystem::recursive_directory_iterator(imageDirectory)) {
    if (entry.is_regular_file() && entry.path().extension() == ".ppm") {
      std::string filename = entry.path().filename().string();
      TrainImages ti;
      ti.filename = entry.path().string(); // full path
      ti.label = GetLabel(filename);
      train_images.push_back(ti);
    }
  }
}

double KNNImageClassifier::ClassificationError(vector<string> test_files) {
  int errors = 0;
  int total = test_files.size();
  for (string &test_file: test_files) {
    string test_label = GetLabel(test_file);
    string predicted_label = Predict(test_file);
    if (predicted_label != test_label)
      errors++;
  }
  return total > 0 ? errors/static_cast<double>(total): 0.0;

}