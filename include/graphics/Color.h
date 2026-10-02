#ifndef COLOR_H_
#define COLOR_H_

#include <string>
#include <algorithm>
using namespace std;


class Color {
public:
  Color(int r, int g, int b, int a) {
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = a;
  }

  int red() const { return color[0]; }
  int green() const { return color[1]; }
  int blue() const { return color[2]; }
  int alpha() const { return color[3]; }

  void setRed(int val) { color[0] = val; }
  void setGreen(int val) { color[1] = val; }
  void setBlue(int val) { color[2] = val; }
  void setAlpha(int val) { color[3] = val; }

  const unsigned char operator[](int index) const {
    return color[index];
  }

  /**
   * @brief Get luminance value using standard formula
   * Uses weighted sum: 0.2126*R + 0.7152*G + 0.0722*B
   * @return Luminance as integer (0-255)
   */
  int getLuminance() const {
    return static_cast<int>(
        0.2126f * color[0] +
        0.7152f * color[1] +
        0.0722f * color[2]
    );
  }

  /**
   * @brief Clamp color values to a range
   * @param min Minimum value
   * @param max Maximum value
   * @return New color with clamped values
   */
  Color clamp(int min, int max) const {
    return Color(
        std::max(min, std::min(max, static_cast<int>(color[0]))),
        std::max(min, std::min(max, static_cast<int>(color[1]))),
        std::max(min, std::min(max, static_cast<int>(color[2]))),
        std::max(min, std::min(max, static_cast<int>(color[3])))
    );
  }

  /**
   * @brief Multiply color by scalar
   */
  Color operator*(float f) const {
    return Color(
        static_cast<int>(color[0] * f),
        static_cast<int>(color[1] * f),
        static_cast<int>(color[2] * f),
        static_cast<int>(color[3] * f)
    ).clamp(0, 255);
  }

  /**
   * @brief Divide color by scalar
   */
  Color operator/(float f) const {
    if (f == 0.0f) return *this;
    return Color(
        static_cast<int>(color[0] / f),
        static_cast<int>(color[1] / f),
        static_cast<int>(color[2] / f),
        static_cast<int>(color[3] / f)
    ).clamp(0, 255);
  }

  /**
   * @brief Add two colors component-wise
   */
  Color operator+(const Color& c) const {
    return Color(
        color[0] + c.color[0],
        color[1] + c.color[1],
        color[2] + c.color[2],
        color[3] + c.color[3]
    ).clamp(0, 255);
  }

  /**
   * @brief Add scalar to all color components
   */
  Color operator+(float f) const {
    int val = static_cast<int>(f);
    return Color(
        color[0] + val,
        color[1] + val,
        color[2] + val,
        color[3] + val
    ).clamp(0, 255);
  }

private:
  unsigned char color[4];
};


#endif