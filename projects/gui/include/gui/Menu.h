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

  /* -------------------------------------------------------------------------------------------------------------- */
  
  typedef void(*gui_menu_callback)(int selectid, int optionid, void* user);

  /* -------------------------------------------------------------------------------------------------------------- */

  
  class Menu : public Widget {
  public:
    Menu(std::string title, int menuid, std::vector<std::string> options, 
         gui_menu_callback cb, void* user);
    void create();
    void onMouseMove(float mx, float my);
    void onMouseRelease(float mx, float my, int button, int modkeys);

  public:
    std::vector<std::string> options;
    int popup_height; 
    int32_t selected_dx;                  /* We can assume that a user will not add more then size_t elements. */
    int32_t draw_dx;                      /* The currently activated element (doesn't need to be the selected one, e.g. in case of release outside.) */

    /* Callback */
    int menu_id;                          /* Callback id, used in callback. */
    gui_menu_callback cb;               /* The callback function that is called when the user selects an option. */
    void* user;                           /* Gets passed to the callback. */
  };

} /* namespace rx */

#endif
