#ifndef REMOXLY_GUI_BUTTON_H
#define REMOXLY_GUI_BUTTON_H

#include <string>
#include <gui/Widget.h>
#include <gui/Types.h>
#include <gui/IconButton.h>

class Button : public Widget {

 public:
  Button(std::string title, int id, unsigned int icon, gui_button_callback cb, void* user, int iconx = 0, int icony = 1);
  bool setup(Gui* g);
  void create();
  void position();
  void setBoundingBox();

  /* interaction */
  void onMousePress(float mx, float my, int button, int modkeys);
  void onMouseRelease(float mx, float my, int button, int modkeys);

 public:
  IconButton icon_button;
};


#endif
