#include "ImageMaker.h"

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
ImageMaker::ImageMaker() {

  // Initialize image dimensions
  width = 0;
  height = 0;

  // Initialize pen color to black
  pen_red = 0;
  pen_green = 0;
  pen_blue = 0;

  // Initialize all image pixels to white
  for (int x = 0; x < MAX_WIDTH; x++) {
    for (int y = 0; y < MAX_HEIGHT; y++) {
      for (COLOR z = RED; z <= BLUE; z = static_cast<COLOR>(z + 1))
        image[x][y][z] = 255;
    }
  }

  max_color = MAX_COLOR;
  SetMagic(MAGIC_NUMBER_COLOR);
}

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
ImageMaker::ImageMaker(string filename) {

  // Initialize all image pixels to white
  for (int x = 0; x < MAX_WIDTH; x++) {
    for (int y = 0; y < MAX_HEIGHT; y++) {
      for (COLOR z = RED; z <= BLUE; z = static_cast<COLOR>(z + 1))
        image[x][y][z] = 255;
    }
  }
  LoadImage(filename);

  // Initialize pen color to black
  pen_red = 0;
  pen_green = 0;
  pen_blue = 0;
}

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
void ImageMaker::LoadImage(string filename) {
  ifstream file(filename);
  if (!file.is_open())
    throw "File failed to open";
  LoadMagic(file);
  LoadDimensions(file);
  LoadMaxColor(file);
  LoadPixels(file);
  file.close();
}

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
void ImageMaker::SaveImage(string filename) {
  ofstream file(filename);
  // Write the image magic number into the given file
  file << GetMagic() << endl;

  // Check whether width and height are nonzero values
  if (GetWidth() == 0 || GetHeight() == 0)
    throw "Image must have non-zero dimensions";

  // Write the width and height into the given file
  file << GetWidth() << "\t" << GetHeight() << endl;

  // Write the image max color pixel value into the given file
  file << GetMaxColor() << endl;

  // Write the pixel values
  for (int x = 0; x < GetHeight(); x++) {
    for (int y = 0; y < GetWidth(); y++) {
      file << image[x][y][RED] << "\t";
      file << image[x][y][GREEN] << "\t";
      file << image[x][y][BLUE] << "\t";
    }
    file << endl;
  }
  file.close();
}

/*
 * Precondition:
 * The width attribute is initialized
 *
 * Postcondition:
 * The width attribute is returned
 */
int ImageMaker::GetWidth() { return width; }

/*
 * Precondition:
 * The height attribute is initialized
 *
 * Postcondition:
 * The height attribute is returned
 */
int ImageMaker::GetHeight() { return height; }

/*
 * Precondition:
 * The width argument is a value between 0 and MAX_WIDTH inclusive
 *
 * Postcondition:
 * 1) Sets the width to a user-defined value
 * 2) Throws a "Width out of bounds" error if width is negative or greater than
 * MAX_WIDTH
 */
void ImageMaker::SetWidth(int width) {
  if (width < 0 || width > MAX_WIDTH)
    throw "Width out of bounds";
  this->width = width;
}

/*
 * Precondition:
 * The height argument is a value between 0 and MAX_HEIGHT inclusive
 *
 * Postcondition:
 * 1) Sets the height to a user-defined value
 * 2) Throws a "Height out of bounds" error if height is negative or greater
 * than MAX_HEIGHT
 */
void ImageMaker::SetHeight(int height) {
  if (height < 0 || height > MAX_HEIGHT)
    throw "Height out of bounds";
  this->height = height;
}

/*
 * Precondition:
 * The pen_red attribute value is initialized
 *
 * Postcondition:
 * The pen_red attribute is returned
 */
int ImageMaker::GetPenRed() { return pen_red; }

/*
 * Precondition:
 * The pen_green attribute value is initialized
 *
 * Postcondition:
 * The pen_green attribute is returned
 */
int ImageMaker::GetPenGreen() { return pen_green; }

/*
 * Precondition:
 * The pen_blue attribute value is initialized
 *
 * Postcondition:
 * The pen_blue attribute is returned
 */
int ImageMaker::GetPenBlue() { return pen_blue; }

/*
 * Precondition:
 * The newR argument is between 0 and the MAX_COLOR inclusive
 *
 * Postcondition:
 * 1) Throws a "Color value invalid" if newR argument is negative or larger than
 * MAX_COLOR
 * 2) Sets the pen_red value to a user-defined value
 */
void ImageMaker::SetPenRed(int newR) {
  if (newR < 0 || newR > MAX_COLOR)
    throw "Color value invalid";
  pen_red = newR;
}

/*
 * Precondition:
 * The newG argument is between 0 and the MAX_COLOR inclusive
 *
 * Postcondition:
 * 1) Throws a "Color value invalid" if newG argument is negative or larger than
 * MAX_COLOR
 * 2) Sets the pen_green value to a user-defined value
 */
void ImageMaker::SetPenGreen(int newG) {
  if (newG < 0 || newG > MAX_COLOR)
    throw "Color value invalid";
  pen_green = newG;
}

/*
 * Precondition:
 * The newB argument is between 0 and the MAX_COLOR inclusive
 *
 * Postcondition:
 * 1) Throws a "Color value invalid" if newB argument is negative or larger than
 * MAX_COLOR
 * 2) Sets the pen_blue value to a user-defined value
 */
void ImageMaker::SetPenBlue(int newB) {
  if (newB < 0 || newB > MAX_COLOR)
    throw "Color value invalid";
  pen_blue = newB;
}

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
void ImageMaker::DrawPixel(int x, int y) {
  PointInBounds(x, y); // Checks whether the coordinate (x, y) is within bounds

  // Color the coordinate (x, y)
  image[y][x][RED] = GetPenRed();
  image[y][x][GREEN] = GetPenGreen();
  image[y][x][BLUE] = GetPenBlue();
}

/*
 * Precondition:
 * row, col, and color arguments are initialized
 *
 * Postcondition:
 * Return pixel value
 */
int ImageMaker::GetPixel(const int row, const int col,
                         const COLOR color) const {
  return image[row][col][color];
}

/*
 * Precondition:
 * 1) The pen_red, pen_green, and pen_blue attributes are initialized
 * 2) The diagonal of the rectangle represented by the coordinates (x1, y1) and
 * (x2, y2) exist
 *
 * Postcondition:
 * 1) Draw a rectangle given the diagonal of the rectangle represented by the
 * coordinates (x1, y1) and (x2, y2)
 * 2) Throw a "Point out of bounds" error if any coordinate in the rectangle is
 * out of bounds
 * 3) Draw a pixel if (x1, y1) and (x2, y2) have the same corresponding values
 */
void ImageMaker::DrawRectangle(int x1, int y1, int x2, int y2) {
  // Check if the endpoints are valid
  PointInBounds(x1, y1);
  PointInBounds(x2, y2);

  // Determine the 4 points used to draw the rectangle
  int p1_x = x1, p1_y = y1;
  int p3_x = x2, p3_y = y2;
  int p2_x = p3_x, p2_y = p1_y;
  int p4_x = p1_x, p4_y = p3_y;

  // Draw the rectangle
  DrawLine(p1_x, p1_y, p2_x, p2_y);
  DrawLine(p2_x, p2_y, p3_x, p3_y);
  DrawLine(p3_x, p3_y, p4_x, p4_y);
  DrawLine(p4_x, p4_y, p1_x, p1_y);
}

/*
 * Precondition:
 * 1) The pen_red, pen_green, and pen_blue attributes are initialized
 * 2) The coordinates (x1, y1) and (x2, y2) exist to draw the line in between
 *
 * Postcondition:
 * 1) Draw a line given coordinates (x1, y1) and (x2, y2)
 * 2) Draw a pixel if coordinates (x1, y1) and (x2, y2) have the same
 * corresponding values
 * 3) Throw "Point out of bounds" if any coordinates in the line to be drawn are
 * out of bounds
 */
void ImageMaker::DrawLine(int x1, int y1, int x2, int y2) {
  // Check if the endpoints are valid
  PointInBounds(x1, y1);
  PointInBounds(x2, y2);

  // Handle same start and end points
  if (x1 == x2 && y1 == y2) {
    DrawPixel(x1, y1);
    return;
  }

  // Handle vertical line
  if (x1 == x2) {
    int start_y = y1 < y2 ? y1 : y2;
    int end_y = y1 > y2 ? y1 : y2;

    // Draw the vertical line
    for (int y = start_y; y <= end_y; y++)
      DrawPixel(x1, y);
    return;
  }

  // Handle sloping line
  // Store the left most point in x1, y1
  // Use y = y1 + m(x-x1) # Point slope form
  int start_x = x1 < x2 ? x1 : x2;
  int start_y = x1 < x2 ? y1 : y2;
  int end_x = x1 > x2 ? x1 : x2;
  int slope = (y2 - y1) / (x2 - x1);

  // Draw the sloping line
  for (int x = start_x; x <= end_x; x++) {
    int y = start_y + slope * (x - start_x);
    DrawPixel(x, y);
  }
}

/*
 * Precondition:
 * The attribute max_color is initialized
 *
 * Postcondition:
 * The attribute max_color is returned
 */
int ImageMaker::GetMaxColor() const { return max_color; }

/*
 * Precondition:
 * The maxColor argument is 255
 *
 * Postcondition:
 * 1) Set the max_color attribute to 255
 * 2) Throw "Max color range not 255" if the max_color attribute isn't 255
 */
void ImageMaker::SetMaxColor(int maxColor) {
  if (maxColor != MAX_COLOR)
    throw "Max color range not 255";
  max_color = maxColor;
}

/*
 * Precondition:
 * The magic argument is P3
 *
 * Postcondition:
 * 1) Set the magic attribute to P3
 * 2) Throw "Bad magic number" if the magic attribute isn't P3
 */
void ImageMaker::SetMagic(string magic) {
  if (magic != MAGIC_NUMBER_COLOR)
    throw "Bad magic number";
  this->magic = magic;
}

/*
 * Precondition:
 * The magic attribute is initialized
 *
 * Postcondition:
 * The magic attribute is returned
 */
string ImageMaker::GetMagic() const { return magic; }

/*
 * Precondition:
 * The coordinate (x, y) exists
 *
 * Postcondition:
 * 1) Return true if (x, y) is within the bounds of the image
 * 2) Throw "Point out of bounds" if (x, y) is out of bounds of the image
 */
bool ImageMaker::PointInBounds(int x, int y) {
  // x is col, y is row
  if (x < 0 || x >= GetWidth() || y < 0 || y >= GetHeight())
    throw "Point out of bounds";
  return true;
}

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
void ImageMaker::LoadMagic(ifstream &file) {
  string magic = "badMagic";
  file >> magic;
  SetMagic(magic);
}

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
void ImageMaker::LoadDimensions(ifstream &file) {
  int width = -1;
  file >> width;
  SetWidth(width);

  int height = -1;
  file >> height;
  SetHeight(height);
}

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
void ImageMaker::LoadMaxColor(ifstream &file) {
  int max_color = -1;
  file >> max_color;
  SetMaxColor(max_color);
}

/*
 * Precondition:
 * 1) The image file has been opened
 * 2) The pen_red, pen_green, and pen_blue attributes have been initialized
 *
 * Postcondition:
 * 1) Store each pixel color value onto the image
 * 2) Throw "Color value invalid" if one of the pixels has a bad value
 */
void ImageMaker::LoadPixels(ifstream &file) {
  for (int x = 0; x < GetHeight(); x++) {
    for (int y = 0; y < GetWidth(); y++) {
      LoadPixel(file);
      image[x][y][RED] = GetPenRed();
      image[x][y][GREEN] = GetPenGreen();
      image[x][y][BLUE] = GetPenBlue();
    }
  }
}

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
void ImageMaker::LoadPixel(ifstream &file) {
  int red = -1, blue = -1, green = -1;
  file >> red >> green >> blue;
  SetPenRed(red);
  SetPenGreen(green);
  SetPenBlue(blue);
}