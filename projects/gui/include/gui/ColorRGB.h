#ifndef REMOXLY_GUI_COLOR_RGB_H
#define REMOXLY_GUI_COLOR_RGB_H

#include <gui/Widget.h>
#include <vector>

namespace rx { 

struct ColorRect {
  float rgba[4];
};

class ColorRGB : public Widget {

 public:
  ColorRGB(std::string title, float* rgb, int ncolors = 50, float sat = 0.8, float val = 1.0);                /* create a basic color slider, with ncolor entries, using saturation level `sat` and value `value` level val. */
  void create();
  void onMousePress(float mx, float my, int button, int modkeys);
  void onMouseRelease(float mx, float my, int button, int modkeys);
  void onMouseMove(float mx, float my);
  
  void setMousePositionValue(int mx);                                                                         /* set the position of the mouse and calculate the correct color index */
  void setPercentageValue(float p);                                                                           /* select the color with a percentage values, between 0 - 1 */

 public:
  std::vector<ColorRect> colors;                                                                              /* we pre calculate the rgb values in the constructor and reuse this data */
  float* rgb;                                                                                                 /* the selected color, wil be copied once the user clicks on the widget */
  float perc_value;                                                                                           /* the percentage index into the colors vector */
  size_t color_dx;                                                                                            /* the selected index from the colors */
  float sat;
  float val;
};

} // namespace rx

#endif
