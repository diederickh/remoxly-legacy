#ifndef REMOXLY_UTILS_H
#define REMOXLY_UTILS_H

#include <string>

/* just a one liner to set the colors in rgba, must be float 4 */
void gui_fill_color(float r, float g, float b, float a, float* rgba);       

/* returns a cleaned up string that can be used for e.g. xml elements */
std::string gui_cleanup_string(std::string str);

/* converts the given string to an int/float */
int gui_string_to_int(std::string str);
float gui_string_to_float(std::string str);
bool gui_string_to_bool(std::string str);

/* color conversion */
void gui_hsv_to_rgb(float h, float s, float v, float& r, float& g, float& b);

 /* clamp the given values between the high/low limits */
template<class T> T gui_clamp(const T& value, const T& low, const T& high);

// -------------------------------------------

template<class T>
T gui_clamp(const T& value, const T& low, const T& high) {
  return value < low ? low : (value > high ? high : value);
}

#endif
