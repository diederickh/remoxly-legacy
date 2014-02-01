/*

  Toggle 
  ------

  A toggle is a set the given value to true or false when the user
  clicks on the widget. 

 */
#ifndef REMOXLY_TOGGLE_H
#define REMOXLY_TOGGLE_H

#include <gui/Widget.h>
#include <gui/Types.h>

class Toggle : public Widget {

 public:
  Toggle(std::string label, bool& value);
  void create();
  void onMousePress(float mx, float my, int button, int modkeys);
  void onMouseRelease(float mx, float my, int button, int modkeys);

 public:
  bool& value;
};

#endif
