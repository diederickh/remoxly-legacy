#include <cmath>
#include <cctype>
#include <algorithm>  // transform
#include <sstream>
#include <gui/Utils.h>

namespace rx { 

void gui_fill_color(float r, float g, float b, float a, float* rgba) {
  rgba[0] = r;
  rgba[1] = g;
  rgba[2] = b;
  rgba[3] = a;
}

std::string gui_cleanup_string(const std::string& str) {

  std::string result;
  bool was_underscore = false;

  for(size_t i = 0; i < str.size(); ++i) {

    if(std::isalnum(str[i])) {
      result.push_back( ::tolower( str[i] ) );
      was_underscore = false;
    }
    else if(!was_underscore && i < (str.size() - 1)){
      result.push_back('_');
      was_underscore = true;
    }
  }

  return result;
}

int gui_string_to_int(const std::string& str) {

	return atoi( str.c_str() );
}

float gui_string_to_float(const std::string& str) {

	return (float)atof( str.c_str() );
}

bool gui_string_to_bool( const std::string& str) {

	return gui_string_to_int( str ) ? true : false;
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

} // namespace rx 
