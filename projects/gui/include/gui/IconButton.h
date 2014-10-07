/*

  IconButton
  ----------

  The icon button is mostly used as a tiny button with an icon 
  from the FontAwesome font. It's a rectangle which calls the given
  `gui_button_callback` function when pressed.  

  In types.h we define a couple of icons that you can use, or you
  can get the icon id from this page: http://fortawesome.github.io/Font-Awesome/cheatsheet/


 */
#ifndef REMOXLY_GUI_ICON_BUTTON_H
#define REMOXLY_GUI_ICON_BUTTON_H

#include <gui/Widget.h>
#include <gui/Types.h>

namespace rx { 

class Group;

class IconButton : public Widget {

 public:
  IconButton(int id, unsigned int icon, gui_button_callback cb = NULL, void* user = NULL, int corners = GUI_CORNER_ALL);
  void setGroup(Group* g);
  void create();
  void call();            /* calls the callback function; can be used by a wrapper widget */

  /* interaction */
  void onMousePress(float mx, float my, int button, int mods);
  void onMouseRelease(float mx, float my, int button, int mods);

 // privately used (needs to be public for (de)serializing.
 public:
  gui_button_callback cb_click;     /* the callback function, see Utils.h where it's defined */
  void* cb_user;                    /* the user pointer that is passed into the callback function */
  int cb_id;                        /* an id specified by the user. the user can pass a different id for each button, but use the same callback function. using this id the user knows what button was clicked */
  unsigned int icon;

 public:
  int icon_w;           
  int icon_h;             
  int icon_x;             /* used to offset the icon position. */
  int icon_y;             /* used to offset the icon position. */ 
  int corners;            /* the corners we need to draw */
};

} // namespace rx

#endif
