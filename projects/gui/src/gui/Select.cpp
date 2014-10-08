#include <gui/Select.h>
#include <gui/Group.h>
#include <gui/Render.h>

namespace rx {

  Select::Select(std::string title, int selectid, std::vector<std::string> options, 
                 gui_menu_callback cb, void* user, 
                 int corners)

    :Widget(GUI_TYPE_SELECT, title)
    ,menu(title, selectid, options, cb, user)
    ,icon_button(0, GUI_ICON_UNSORTED, NULL, NULL, (corners & ~GUI_CORNER_LEFT))
    ,corners(corners)
    ,options(options)
  {
    h = 22;
    icon_button.icon_x = 1;
    icon_button.icon_y = 0;
    menu.hide();
  }

  void Select::setGroup(Group* g) {
    Widget::setGroup(g);
    add(&icon_button, g);
    add(&menu, g);
  }

  void Select::create() {
    /* @todo use state color func for label. */
    int bt_click_offset = 0;
    int corn_sel = corners;
    int corn_off = 0;
    int corn_icon = icon_button.corners;
    float* label_color = group->label_color;

    corn_sel = (corners & ~GUI_CORNER_RIGHT);

    if(state & GUI_STATE_DOWN_INSIDE) {
      bt_click_offset = 1;
      label_color = group->number_color;

      if (GUI_DIRECTION_DOWN == menu.direction) {
        corn_off = GUI_CORNER_BOTTOM;
        corn_sel &= ~GUI_CORNER_BOTTOM;
      }
      else {
        corn_off = GUI_CORNER_TOP;
        corn_sel &= ~GUI_CORNER_TOP;
      }
    }

    icon_button.corners = (corners & ~GUI_CORNER_LEFT) & ~corn_off;
    
    render->addRoundedRectangle(x, y, w - icon_button.w, h, 6.0, group->getButtonStateColor(this), true, group->shade_top, group->shade_bottom, corn_sel);

    if (menu.selected_dx == -1) {
      render->writeText(x + group->xindent, y + group->yindent + bt_click_offset, label, label_color);
    }
    else {
      render->writeText(x + group->xindent, y + group->yindent + bt_click_offset, menu.options[(uint32_t)menu.selected_dx], label_color);
    }
  }

  void Select::position() {

    icon_button.x = x + (w - icon_button.w);
    icon_button.y = y;
    menu.x = x;
    menu.y = y;
    menu.w = w;
  }

  void Select::setBoundingBox() {

    if (!isDrawn()) {
      bbox[0] = 0;
      bbox[1] = 0;
      bbox[2] = 0;
      bbox[3] = 0;
      return;
    }

    bbox[0] = x;
    bbox[1] = y;
    bbox[2] = w;
    bbox[3] = h;
  }

  void Select::onMousePress(float mx, float my, int button, int modkeys) {
  
    Widget::onMousePress(mx, my, button, modkeys);

    if(GUI_IS_INSIDE_WIDGET(this, mx, my)) {
      state |= GUI_STATE_DOWN_INSIDE;
      icon_button.state |= GUI_STATE_DOWN_INSIDE;
      menu.show();
      needs_redraw = true;
    }
  }

  void Select::onMouseRelease(float mx, float my, int button, int modkeys) {

    Widget::onMouseRelease(mx, my, button, modkeys);

    if(state & GUI_STATE_DOWN_INSIDE) {
      needs_redraw = true;
      notify(GUI_EVENT_VALUE_CHANGED); 
    }

    menu.hide();
    state &= ~GUI_STATE_DOWN_INSIDE;
    icon_button.state &= ~GUI_STATE_DOWN_INSIDE;
  }
     
} /* namespace rx */
