/*

  Menu
  ----

  Shows a 'popup' with items in a list that can be selected. For now its just a
  very simple drop down list. It's used by `Select` to draw the selectable items.
  Look at `Select` when you want to use the menu in your own widget somewhere. 
  Some things to keep in mind (noted while creating the select).
  
  - When you use Menu as a child element make sure that it receives all the 
    mouse events. e.g. when you `hide()` the menu it means that it won't receive
    mouse events anymore, see how `Select` first calls `Widget::onMouseRelease()`
    before calling `menu.hide()`. This order is important.
  

 */
#ifndef REMOXLY_GUI_MENU_H
#define REMOXLY_GUI_MENU_H

#include <string>
#include <vector>
#include <gui/Widget.h>
#include <gui/IconButton.h>

namespace rx {
  
  class Menu : public Widget {
  public:
    Menu(std::string title, std::vector<std::string> options);
    void create();
    void onMouseMove(float mx, float my);
    void onMouseRelease(float mx, float my, int button, int modkeys);
  public:
    std::vector<std::string> options;
    int popup_height; 
    int32_t selected_dx; /* we can assume that a user will not add more then size_t elements. */
    int32_t draw_dx;
  };

} /* namespace rx */

#endif
