#ifndef REMOXLY_GUI_BUTTON_H
#define REMOXLY_GUI_BUTTON_H

#include <string>
#include <gui/Widget.h>
#include <gui/Types.h>
#include <gui/IconButton.h>

namespace rx { 

class Button : public Widget {

 public:
  Button(std::string title, int id, unsigned int icon, gui_button_callback cb, void* user = NULL, int style = GUI_CORNER_ALL, int iconx = 1, int icony = 1);
  void setGroup(Group* g);
  void create();
  void position();
  void setBoundingBox();
  void call();                  /* force a call of the button */

  /* interaction */
  void onMousePress(float mx, float my, int button, int modkeys);
  void onMouseRelease(float mx, float my, int button, int modkeys);

 public:
  IconButton icon_button;
};

} // namespace rx 

#endif
