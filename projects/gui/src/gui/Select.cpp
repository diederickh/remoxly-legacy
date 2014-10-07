#include <gui/Select.h>
#include <gui/Group.h>
#include <gui/Render.h>

namespace rx {

  static void on_icon_clicked(int id, void* user);

  Select::Select(std::string title, std::vector<std::string> options, int cbid)
    :Widget(GUI_TYPE_SELECT, title)
    ,menu(title, options)
    ,icon_button(0, GUI_ICON_UNSORTED, on_icon_clicked, this, GUI_CORNER_RIGHT)
    ,cb_id(cbid)
    ,options(options)
  {
    h = 22;
    icon_button.icon_x = 1;
    icon_button.icon_y = 0;
    depth = 100;
    menu.hide();
  }

  void Select::setGroup(Group* g) {
    Widget::setGroup(g);
    add(&icon_button, g);
    add(&menu, g);
  }

  void Select::create() {
    int bt_click_offset = 0;
    if(state & GUI_STATE_DOWN_INSIDE) {
      bt_click_offset = 1;
    }

    //    render->setLayer(10);
    render->addRoundedRectangle(x, y, w - icon_button.w, h, 6.0, group->getButtonStateColor(this), true, GUI_CORNER_NONE);

    if (menu.selected_dx == -1) {
      render->writeText(x + group->xindent, y + group->yindent + bt_click_offset, label, group->label_color);
    }
    else {
      render->writeText(x + group->xindent, y + group->yindent + bt_click_offset, menu.options[(uint32_t)menu.selected_dx], group->label_color);
    }
    //    render->setLayer(0);
  }

  void Select::position() {
    icon_button.x = x + (w - icon_button.w);
    icon_button.y = y;
    menu.x = x;
    menu.y = y;
    menu.w = w;
  }

  void Select::setBoundingBox() {
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

  static void on_icon_clicked(int id, void* user) {
    printf("ID: %d", id);
  }
     
} /* namespace rx */
