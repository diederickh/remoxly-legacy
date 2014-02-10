/*

  Text 
  ------

  Text input

 */
#ifndef REMOXLY_GUI_TEXT_H
#define REMOXLY_GUI_TEXT_H

#include <gui/Widget.h>
#include <gui/Types.h>
#include <string>

class Text : public Widget {

 public:
  Text(std::string label, std::string& value, int textw = 200);
  void create();

  /* interaction */
  void onCharPress(unsigned int key);
  void onKeyPress(int key, int modkeys);
  void onMousePress(float mx, float my, int button, int modkeys);
  void onMousePressOutside(float mx, float my, int button, int modkeys);

  /* text input specific */
  void disableInputAndCopyValue();

 public:
  std::string& value;
  int text_x;
  int text_w;
};

#endif
