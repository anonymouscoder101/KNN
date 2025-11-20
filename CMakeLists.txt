cmake_minimum_required(VERSION 3.17)
project(ImageMaker)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)

# Setup the Catch2 library directly from git
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.6.0
)
FetchContent_MakeAvailable(Catch2)

# Increase the stack memory.  ppm images are big!
MATH(EXPR stack_size "10 * 1024 * 1024") # 10 Mb
# Windows settings
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")
endif()

# Apple settings - Can be removed if you don't declare a lot of ImageMaker objects
if (APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-stack_size,10000000")
endif()

# Linux settings
if(UNIX AND NOT APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,stack-size=${stack_size}")
endif()

# Set output directories for executables to ensure they are easy to find and run
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Run basic driver test showing functionality.
add_executable(ImageMaker_Driver ImageMaker_driver.cpp ImageMaker.cpp)

# Unit test target for KNNImageClassifier
file(GLOB UNIT_TEST_SOURCES unit_tests/*.cpp)
add_executable(KNNImageClassifier_Tests ${UNIT_TEST_SOURCES} ImageMaker.cpp KNNImageClassifier.cpp)
target_link_libraries(KNNImageClassifier_Tests PRIVATE Catch2::Catch2WithMain)
#target_include_directories(KNNImageClassifier_Tests PRIVATE ${CMAKE_BINARY_DIR}/_deps/catch2-src/include)

# Ensure tests are found in the correct location
#add_test(NAME KNNImageClassifier_Tests COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/KNNImageClassifier_Tests)

#enable_testing()
