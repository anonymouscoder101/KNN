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

# Run basic driver test showing functionality.
add_executable(ImageMaker_Driver ImageMaker_driver.cpp ImageMaker.cpp)

# Test getters, setters, constructors, save and load
add_executable(Getter_Setter_Test unit_tests/getter_setter_tests.cpp ImageMaker.cpp)
add_executable(Constructor_Test unit_tests/constructor_tests.cpp ImageMaker.cpp
        KNNImageClassifier.cpp
        KNNImageClassifier.h)
add_executable(Save_Load_Test unit_tests/save_load_tests.cpp ImageMaker.cpp)

# Draw Tests (requires getters, setters, constructors, save and load to work)
add_executable(DrawPixel_Test unit_tests/DrawPixel_tests.cpp ImageMaker.cpp)
add_executable(DrawRectangle_Test unit_tests/DrawRectangle_tests.cpp ImageMaker.cpp)
add_executable(DrawLine_Test unit_tests/DrawLine_tests.cpp ImageMaker.cpp)

target_link_libraries(Getter_Setter_Test Catch2::Catch2WithMain)
target_link_libraries(Constructor_Test Catch2::Catch2WithMain)
target_link_libraries(Save_Load_Test Catch2::Catch2WithMain)
target_link_libraries(DrawPixel_Test Catch2::Catch2WithMain)
target_link_libraries(DrawRectangle_Test Catch2::Catch2WithMain)
target_link_libraries(DrawLine_Test Catch2::Catch2WithMain)

# Copy images from backup_images to images.  PPM files in images is used for catch tests.
add_custom_target(copy_image_data
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/backup_images/"
        "${CMAKE_CURRENT_SOURCE_DIR}/images/"
        COMMENT "Copying image data for tests"
)
add_dependencies(ImageMaker_Driver copy_image_data)
add_dependencies(Getter_Setter_Test copy_image_data)
add_dependencies(Constructor_Test copy_image_data)
add_dependencies(Save_Load_Test copy_image_data)
add_dependencies(DrawPixel_Test copy_image_data)
add_dependencies(DrawRectangle_Test copy_image_data)
add_dependencies(DrawLine_Test copy_image_data)