#include <cmath>
#include <cctype>
#include <algorithm>  // transform
#include <sstream>
#include <gui/Utils.h>

void gui_fill_color(float r, float g, float b, float a, float* rgba) {
  rgba[0] = r;
  rgba[1] = g;
  rgba[2] = b;
  rgba[3] = a;
}

std::string gui_cleanup_string(std::string str) {

  std::string result;

  for(size_t i = 0; i < str.size(); ++i) {

    if(std::isalnum(str[i])) {
      result.push_back(str[i]);
    }
    else {
      result.push_back('_');
    }
  }

  std::transform(result.begin(), result.end(), result.begin(), ::tolower);
  return result;
}

int gui_string_to_int(std::string str) {

  int result = 0;
  std::stringstream ss;
  ss << str;
  ss >> result;
  return result;
}

float gui_string_to_float(std::string str) {

  float result = 0.0f;
  std::stringstream ss;
  ss << str;
  ss >> result;
  return result;
}

bool gui_string_to_bool(std::string str) {

  bool result = false;
  std::stringstream ss;
  ss << str;
  ss >> result;
  return result;
}

void gui_hsv_to_rgb(float h, float s, float v, float& r, float& g, float& b) {
  float tmp_r = gui_clamp<float>((-1.0f + fabs(6.0f * h - 3.0f)), 0,1);
  float tmp_g = gui_clamp<float>(( 2.0f - fabs(6.0f * h - 2.0f)), 0,1);      
  float tmp_b = gui_clamp<float>(( 2.0f - fabs(6.0f * h - 4.0f)), 0,1);  
  float p = 1.0f - s;  
  r = v * (p + tmp_r * s);  
  g = v * (p + tmp_g * s);    
  b = v * (p + tmp_b * s);        
}
