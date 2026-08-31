#include "Image.h"

#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>


Image::Image() : width(0), height(0), components(0), pixels(nullptr) {}

Image::Image(const unsigned char* buffer, int len) {
  unsigned char *data = stbi_load_from_memory(buffer, len, &width, &height, &components, STBI_rgb_alpha);

  // Check if image loading failed
  if (data == nullptr) {
    // stbi_load failed (file not found, invalid format, etc.)
    width = 0;
    height = 0;
    components = 0;
    pixels = nullptr;
    return;
  }

  components = 4;
  pixels = new unsigned char[width * height * components];
  std::copy(data, data + width * height * components, pixels);
  stbi_image_free(data);
}

Image::Image(const std::string &filename) {
  unsigned char *data =
      stbi_load(filename.c_str(), &width, &height, &components, STBI_rgb_alpha);

  // Check if image loading failed
  if (data == nullptr) {
    // stbi_load failed (file not found, invalid format, etc.)
    width = 0;
    height = 0;
    components = 0;
    pixels = nullptr;
    return;
  }

  components = 4;
  pixels = new unsigned char[width * height * components];
  std::copy(data, data + width * height * components, pixels);
  stbi_image_free(data);
}

Image::Image(int width, int height)
    : width(width), height(height), components(4) {
  pixels = new unsigned char[width * height * components];
}

Image::Image(const Image &image) {
  this->width = image.width;
  this->height = image.height;
  this->components = image.components;
  this->pixels = new unsigned char[width * height * components];
  std::copy(image.pixels, image.pixels + width * height * components, pixels);
}

Image::~Image() { delete[] pixels; }

int Image::getWidth() const { return width; }

int Image::getHeight() const { return height; }

Color Image::getPixel(int x, int y) const {
  // Clamp coordinates to image boundaries
  // This allows filters to access edge pixels without special handling
  x = std::max(0, std::min(x, width - 1));
  y = std::max(0, std::min(y, height - 1));

  unsigned char *pixel = &pixels[(x + width * y) * components];
  return Color(pixel[0], pixel[1], pixel[2], pixel[3]);
}

void Image::setPixel(int x, int y, const Color &color) {
  unsigned char *pixel = &pixels[(x + width * y) * components];
  pixel[0] = color[0];
  pixel[1] = color[1];
  pixel[2] = color[2];
  pixel[3] = color[3];
}

void Image::saveAs(const std::string &filename) const {
  stbi_write_png(filename.c_str(), width, height, components, pixels,
                 width * components);
}

void Image::operator=(const Image &image) {
  this->width = image.width;
  this->height = image.height;
  this->components = image.components;
  delete[] this->pixels;
  this->pixels = new unsigned char[width * height * components];
  std::copy(image.pixels, image.pixels + width * height * components, pixels);
}
