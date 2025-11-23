#include <catch2/catch_test_macros.hpp>
#include "../KNNImageClassifier.h"
#include <filesystem>
#include <vector>
#include <string>
#include <random>
#include <algorithm>

namespace fs = std::filesystem;

// Helper: Recursively collect all .ppm files from a directory
auto collect_ppm_files(const std::string& root) -> std::vector<std::string> {
    std::vector<std::string> files;
    for (auto& p : fs::recursive_directory_iterator(root)) {
        if (p.is_regular_file() && p.path().extension() == ".ppm") {
            files.push_back(p.path().string());
        }
    }
    return files;
}

// Fixed random seed for reproducibility
constexpr unsigned int FIXED_SEED = 301;

TEST_CASE("KNNImageClassifier public methods", "[KNN]") {
    std::string image_dir = "../../jellybean_crops";
    std::string test_dir = "../../test_images";
    int k = 3;
    KNNImageClassifier classifier(image_dir, k);

    SECTION("GetLabel returns correct label format") {
        auto files = collect_ppm_files(image_dir);
        REQUIRE(!files.empty());
        std::string label = classifier.GetLabel(files[0]);
        REQUIRE(!label.empty());
    }

    SECTION("EuclideanDistance returns non-negative value") {
        auto files = collect_ppm_files(image_dir);
        REQUIRE(files.size() > 1);
        ImageMaker img1(files[0]);
        ImageMaker img2(files[1]);
        double dist = classifier.EuclideanDistance(img1, img2);
        REQUIRE(dist >= 0.0);
    }

    SECTION("Predict returns a label from training set") {
        auto files = collect_ppm_files(test_dir);
        REQUIRE(!files.empty());
        std::string label = classifier.Predict(files[0]);
        REQUIRE(!label.empty());
    }

    SECTION("ClassificationError computes overall error rate (fixed seed)") {
        auto test_files = collect_ppm_files(test_dir);
        REQUIRE(!test_files.empty());
        std::mt19937 rng(FIXED_SEED);
        std::shuffle(test_files.begin(), test_files.end(), rng);
        double error_rate = classifier.ClassificationError(test_files);
        REQUIRE(error_rate >= 0.0);
        REQUIRE(error_rate <= 1.0);
    }
}

