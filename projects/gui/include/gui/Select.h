#ifndef REMOXLY_GUI_SELECT_H
#define REMOXLY_GUI_SELECT_H

#include <string>
#include <vector>
#include <gui/Widget.h>
#include <gui/IconButton.h>
#include <gui/Menu.h>

namespace rx {   

  class Select : public Widget, public MenuListener { 
  public:
    Select(std::string title, int selectid, std::vector<std::string> options, gui_menu_callback cb, void* user,int style = GUI_STYLE_NONE);
    void setGroup(Group* g);
    void create();
    void position();
    void setBoundingBox();
    void setDirection(int dir);

    /* Menu listener */
    void onMenuShouldClose();
    void onMenuSelected(int32_t menuid, int32_t optionid);

    /* Interaction */
    void onMousePress(float mx, float my, int button, int modkeys);
    void onMouseRelease(float mx, float my, int button, int modkeys);

  public:
    Menu menu;
    IconButton icon_button;
    std::vector<std::string> options;     /* The options that are darwn. */
  };

  /* -------------------------------------------------------------------------------------------------------------- */

  inline void Select::setDirection(int dir) {
    menu.setDirection(dir);
  }

} /* namespace rx */

#endif
