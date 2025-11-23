//
// Assignment Created by Varick Erickson
//

#ifndef IMAGEMAKER_H
#define IMAGEMAKER_H

#include <cmath>
#include <fstream>
#include <string>

const int MAX_WIDTH = 800;
const int MAX_HEIGHT = 800;
const int MAX_COLOR = 255;
const std::string MAGIC_NUMBER_COLOR = "P3";
enum COLOR { RED, GREEN, BLUE };

using namespace std;

//  Please put your pre and post comments in this file. See page 139 in the
//  textbook.

class ImageMaker {
  friend class KNNImageClassifier;
public:
  /*
   * Precondition:
   * None
   *
   * Postcondition:
   * 1) Initializes the image height and width to 0
   * 1) Sets initial pen color to black
   * 3) Initializes all image pixels to white
   * 4) Initializes the max image pixel color to 255
   * 5) Sets the magic number to P3
   */
  ImageMaker();

  /*
   * Precondition:
   * None
   *
   * Postcondition:
   * 1) Initializes all image pixels to white
   * 2) Loads a ppm image into the image matrix
   * 3) Sets the width, height, magic number, max image pixel color to the
   * corresponding file attributes
   * 4) Throws a "File failed to open" if the file fails to open
   * 5) Throws a "Bad magic number" if the image magic number is not P3
   * 6) Throws a "Width out of bounds" if the width is out of bounds
   * 7) Throws a "Height out of bounds" if the height is out of bounds
   * 8) Throws a "Max color range not 255" if the maximum color value isn't 255
   * 9) Throws a "Color value invalid" if one of the pixels has a negative value
   * or value greater than the maximum possible value
   */
  ImageMaker(string filename);

  // --Opens image with filename and stores information into--

  /*
   * Precondition:
   * The ppm image file (filename argument) exists
   *
   * Postcondition:
   * 1) The image data is stored into the image, width, and height attributes
   * 2) Throws a "File failed to open" if the file fails to open
   * 3) Throws a "Bad magic number" if the image magic number is not P3
   * 4) Throws a "Width out of bounds" if the width is out of bounds
   * 5) Throws a "Height out of bounds" if the height is out of bounds
   * 6) Throws a "Max color range not 255" if the maximum color value isn't 255
   * 7) Throws a "Color value invalid" if one of the pixels has a negative value
   * or value greater than the maximum possible value
   *
   */
  void LoadImage(string filename);

  /*
   * Precondition:
   * 1) The ppm image file (filename argument) exists
   * 2) Image max color pixel value is always 255
   * 3) Image magic number is always P3
   *
   * Postcondition:
   * 1) Writes out the RGB values from image attribute to the given output file
   * 2) Throws "image must have non-zero dimensions" if width or height is 0
   */
  void SaveImage(string filename);

  // --Size functions--

  /*
   * Precondition:
   * The width attribute is initialized
   *
   * Postcondition:
   * The width attribute is returned
   */
  int GetWidth();

  /*
   * Precondition:
   * The height attribute is initialized
   *
   * Postcondition:
   * The height attribute is returned
   */
  int GetHeight();

  /*
   * Precondition:
   * The width argument is a value between 0 and MAX_WIDTH inclusive
   *
   * Postcondition:
   * 1) Sets the width to a user-defined value
   * 2) Throws a "Width out of bounds" error if width is negative or greater
   * than MAX_WIDTH
   */
  void SetWidth(int width);

  /*
   * Precondition:
   * The height argument is a value between 0 and MAX_HEIGHT inclusive
   *
   * Postcondition:
   * 1) Sets the height to a user-defined value
   * 2) Throws a "Height out of bounds" error if height is negative or greater
   * than MAX_HEIGHT
   */
  void SetHeight(int height);

  // --Color functions--

  /*
   * Precondition:
   * The pen_red attribute value is initialized
   *
   * Postcondition:
   * The pen_red attribute is returned
   */
  int GetPenRed();

  /*
   * Precondition:
   * The pen_green attribute value is initialized
   *
   * Postcondition:
   * The pen_green attribute is returned
   */
  int GetPenGreen();

  /*
   * Precondition:
   * The pen_blue attribute value is initialized
   *
   * Postcondition:
   * The pen_blue attribute is returned
   */
  int GetPenBlue();

  /*
   * Precondition:
   * The newR argument is between 0 and the MAX_COLOR inclusive
   *
   * Postcondition:
   * 1) Throws a "Color value invalid" if newR argument is negative or larger
   * than MAX_COLOR
   * 2) Sets the pen_red value to a user-defined value
   */
  void SetPenRed(int newR);

  /*
   * Precondition:
   * The newG argument is between 0 and the MAX_COLOR inclusive
   *
   * Postcondition:
   * 1) Throws a "Color value invalid" if newG argument is negative or larger
   * than MAX_COLOR
   * 2) Sets the pen_green value to a user-defined value
   */
  void SetPenGreen(int newG);

  /*
   * Precondition:
   * The newB argument is between 0 and the MAX_COLOR inclusive
   *
   * Postcondition:
   * 1) Throws a "Color value invalid" if newB argument is negative or larger
   * than MAX_COLOR
   * 2) Sets the pen_blue value to a user-defined value
   */
  void SetPenBlue(int newB);

  /*
   * Precondition:
   * The magic attribute is initialized
   *
   * Postcondition:
   * The magic attribute is returned
   */
  string GetMagic() const;

  /*
   * Precondition:
   * The magic argument is P3
   *
   * Postcondition:
   * 1) Set the magic attribute to P3
   * 2) Throw "Bad magic number" if the magic attribute isn't P3
   */
  void SetMagic(string magic);

  // --Drawing methods--
  // -These methods will use the current red, green, blue values of the pen-

  /*
   * Precondition:
   * 1) The pen_red, pen_green, and pen_blue attributes are initialized
   * 2) The coordinate to be colored (x, y) exists
   *
   * Postcondition:
   * 1) Color the coordinate (x, y)
   * 2) Throw "Point out of bounds" if a coordinate is out of bounds aka if the
   * coordinate (x, y) is invalid
   */
  void DrawPixel(int x, int y);

  /*
   * Precondition:
   * row, col, and color arguments are initialized
   *
   * Postcondition:
   * Return pixel value
   */
  int GetPixel(int row, int col, COLOR color) const;

  /*
   * Precondition:
   * 1) The pen_red, pen_green, and pen_blue attributes are initialized
   * 2) The diagonal of the rectangle represented by the coordinates (x1, y1)
   * and (x2, y2) exist
   *
   * Postcondition:
   * 1) Draw a rectangle given the diagonal of the rectangle represented by the
   * coordinates (x1, y1) and (x2, y2)
   * 2) Throw a "Point out of bounds" error if any coordinate in the rectangle
   * is out of bounds 3) Draw a pixel if (x1, y1) and (x2, y2) have the same
   * corresponding values
   */
  void DrawRectangle(int x1, int y1, int x2, int y2);

  /*
   * Precondition:
   * 1) The pen_red, pen_green, and pen_blue attributes are initialized
   * 2) The coordinates (x1, y1) and (x2, y2) exist to draw the line in between
   *
   * Postcondition:
   * 1) Draw a line given coordinates (x1, y1) and (x2, y2)
   * 2) Draw a pixel if coordinates (x1, y1) and (x2, y2) have the same
   * corresponding values
   * 3) Throw "Point out of bounds" if any coordinates in the line to be drawn
   * are out of bounds
   */
  void DrawLine(int x1, int y1, int x2, int y2);

  /*
   * Precondition:
   * The attribute max_color is initialized
   *
   * Postcondition:
   * The attribute max_color is returned
   */
  int GetMaxColor() const;

  /*
   * Precondition:
   * The maxColor argument is 255
   *
   * Postcondition:
   * 1) Set the max_color attribute to 255
   * 2) Throw "Max color range not 255" if the max_color attribute isn't 255
   */
  void SetMaxColor(int max_color);

private:
  string magic;
  int max_color;
  int width;
  int height;
  int pen_red;                      // Used by draw functions
  int pen_green;                    // Used by draw functions
  int pen_blue;                     // Used by draw functions
  bool PointInBounds(int x, int y); // Not a bad idea to implement

  //   x          y
  short image[MAX_WIDTH][MAX_HEIGHT][3];

  /*
   * Precondition:
   * 1) The image file has been opened
   * 2) The image file has an initialized magic attribute
   *
   * Postcondition:
   * 1) The magic attribute read from the file is P3
   * 2) Throw "Bad magic number" if the magic number isn't P3
   * 3) Set the magic attribute to P3
   */
  void LoadMagic(ifstream &file);

  /*
   * Precondition:
   * 1) The image file has been opened
   * 2) The image file contains initialized width and height attributes
   *
   * Postcondition:
   * 1) The width and height attributes read from the file are within bounds
   * 2) Throw "Width out of bounds" if the width attribute is out of bounds
   * 3) Throw "Height out of bounds" if the height attribute is out of bounds
   * 4) Set the width and height attribute to the corresponding file attributes
   */
  void LoadDimensions(ifstream &file);

  /*
   * Precondition:
   * 1) The image file has been opened
   * 2) The image file has an initialized max_color attribute
   *
   * Postcondition:
   * 1) The max_color attribute read from the file is 255
   * 2) Throw "Max color range not 255" if the max_color attribute isn't 255
   * 3) Set the max_color attribute to 255
   */
  void LoadMaxColor(ifstream &file);

  /*
   * Precondition:
   * 1) The image file has been opened
   * 2) The pen_red, pen_green, and pen_blue attributes have been initialized
   *
   * Postcondition:
   * 1) Store each pixel color value onto the image
   * 2) Throw "Color value invalid" if one of the pixels has a bad value
   */
  void LoadPixels(ifstream &file);

  /*
   * Precondition:
   * 1) The image file has been opened
   * 2) The image file contains initialized pen_red, pen_green, and pen_blue
   * attributes
   *
   * Postcondition:
   * 1) The pen_red, pen_green, and pen_blue attributes read from the file are
   * within bounds
   * 2) Throw "Color value invalid" if one of the attributes pen_red, pen_green,
   * or pen_blue have a bad value
   * 3) Set the pen_red, pen_green, and pen_blue attributes to the corresponding
   * file attributes
   */
  void LoadPixel(ifstream &file);
};

#endif // IMAGEMAKER_H
