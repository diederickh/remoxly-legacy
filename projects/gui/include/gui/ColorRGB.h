#ifndef REMOXLY_COLOR_RGB_H
#define REMOXLY_COLOR_RGB_H

#include <gui/Widget.h>

class ColorRGB : public Widget {

 public:
  ColorRGB(std::string title, float* rgb);
  void create();
  void onMousePress(float mx, float my, int button, int modkeys);
  void onMouseRelease(float mx, float my, int button, int modkeys);
  void onMouseMove(float mx, float my);

 public:
  float* rgb;
};

#endif
