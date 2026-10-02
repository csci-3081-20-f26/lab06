#ifndef IMAGE_H_
#define IMAGE_H_

#include "Color.h"
#include <string>

class Image {
public:
  // constructors
  Image();
  Image(const unsigned char* buffer, int len);
  Image(const std::string &filename);
  Image(int width, int height);
  Image(const Image &image);
  ~Image();

  // methods
  void saveAs(const std::string &filename) const;
  int getWidth() const;
  int getHeight() const;
  Color getPixel(int x, int y) const;
  void setPixel(int x, int y, const Color &color);
  const unsigned char *getData() const { return pixels; }
  unsigned char *getData() { return pixels; }
  void operator=(const Image &image);

private:
  int width;
  int height;
  int components;
  unsigned char *pixels;
};

#endif // IMAGE_H_
