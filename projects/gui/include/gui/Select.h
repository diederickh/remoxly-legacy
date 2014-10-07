#ifndef REMOXLY_GUI_SELECT_H
#define REMOXLY_GUI_SELECT_H

#include <string>
#include <vector>
#include <gui/Widget.h>
#include <gui/IconButton.h>
#include <gui/Menu.h>

namespace rx {   
  
  class Select : public Widget { 
  public:
    Select(std::string title, std::vector<std::string> options, int id);
    //    ~Select();
    void setGroup(Group* g);
    void create();
    void position();
    void setBoundingBox();

    /* interaction */
    void onMousePress(float mx, float my, int button, int modkeys);
    void onMouseRelease(float mx, float my, int button, int modkeys);

  public:
    Menu menu;
    IconButton icon_button;
    std::vector<std::string> options;     /* The options that are darwn. */
    int cb_id;                            /* Callback id, used in callback. */
  };

} /* namespace rx */

#endif
