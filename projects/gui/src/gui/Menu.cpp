#include <gui/Menu.h>
#include <gui/Group.h>
#include <gui/Render.h>

namespace rx {

  Menu::Menu(std::string title, int menuid, std::vector<std::string> options, gui_menu_callback cb, void* user, MenuListener* listener)
    :Widget(GUI_TYPE_MENU, title)
    ,options(options)
    ,selected_dx(-1)
    ,draw_dx(-1)
    ,direction(GUI_DIRECTION_DOWN)
    ,menu_id(menuid)
    ,cb(cb)
    ,user(user)
    ,listener(listener)
  {
    h = 22;
    popup_height = options.size() * h;
  }

  void Menu::create() {

    int yy;
    size_t last = options.size() - 1;
    
    render->setLayer(1);

    if (GUI_DIRECTION_DOWN == direction) {

      /* Downwards. */
      yy = y + h;
      render->addRoundedRectangle(x, yy, w, popup_height, 6.0, group->header_color, true, group->shade_top, group->shade_bottom, GUI_CORNER_BOTTOM);

      if (style & GUI_OUTLINE) {
        render->addRoundedRectangle(x + 0.5, yy, w - 1.0, popup_height, 6.0, group->line_color, false, group->shade_top, group->shade_bottom, GUI_CORNER_BOTTOM);
      }

      for (size_t i = 0; i < options.size(); ++i) {
        if (GUI_IS_INSIDE(mouse_x, mouse_y, (x + group->xindent), (yy + group->yindent), w, h)) {
          render->addRoundedRectangle(x, yy, w, h, 6.0, group->selected_color, true, group->shade_top, group->shade_bottom, (i == last) ? GUI_CORNER_BOTTOM : GUI_STYLE_NONE);
          render->writeText(x + group->xindent, yy + group->yindent, options[i], group->label_color);
          draw_dx = (int32_t)i;
        }
        else {
          render->writeText(x + group->xindent, yy + group->yindent, options[i], group->number_color);
        }

        yy += h;
      }
    }
    else {

      /* Upwards. */
      yy = y;
      render->addRoundedRectangle(x, yy - popup_height, w, popup_height, 6.0, group->header_color, true, group->shade_top, group->shade_bottom, GUI_CORNER_TOP);

      if (style & GUI_OUTLINE) {
        render->addRoundedRectangle(x + 0.5, yy - popup_height, w - 1.0, popup_height, 6.0, group->line_color, false, group->shade_top, group->shade_bottom, GUI_CORNER_TOP);
      }

      yy = y - popup_height;

      for (size_t i = 0; i < options.size(); ++i) {
        if (GUI_IS_INSIDE(mouse_x, mouse_y, x, yy, w, h)) {
          render->addRoundedRectangle(x, yy, w, h, 6.0, group->selected_color, true, group->shade_top, group->shade_bottom, (i == 0) ? GUI_CORNER_TOP : GUI_STYLE_NONE);
          render->writeText(x + group->xindent, yy + group->yindent, options[i], group->label_color);
          draw_dx = (int32_t)i;
        }
        else {
          render->writeText(x + group->xindent, yy + group->yindent, options[i], group->number_color);
        }

        yy += h;
      }
    }

    render->setLayer(0);
  }

  void Menu::onMouseMove(float mx, float my) {

    Widget::onMouseMove(mx, my);

    if (state & GUI_STATE_HIDDEN) {
      return;
    }

    if (GUI_DIRECTION_DOWN == direction) { 
      if (GUI_IS_INSIDE(mx, my, x, y, w, (h + popup_height))) {
        needs_redraw = true;
      }
      else {
        listener->onMenuShouldClose();
      }
    }
    else {
      if (GUI_IS_INSIDE(mx, my, x, y - popup_height, w, (h + popup_height))) {
        needs_redraw = true;
      }
      else {
        listener->onMenuShouldClose();
      }
    }
  }

  void Menu::onMouseRelease(float mx, float my, int button, int modkeys) {

    if (GUI_DIRECTION_DOWN == direction) { 

      if (GUI_IS_INSIDE(mx, my, x, y+h, w, (popup_height))) {

        selected_dx = draw_dx;

        if (NULL != cb) {
          cb(menu_id, selected_dx, user);
        }

        listener->onMenuSelected(menu_id, selected_dx);
      }
    }
    else {

      if (GUI_IS_INSIDE(mx, my, x, y - popup_height, w, (popup_height))) {

        selected_dx = draw_dx;

        if (NULL != cb) {
          cb(menu_id, selected_dx, user);
        }

        listener->onMenuSelected(menu_id, selected_dx);
      }
    }

    Widget::onMouseRelease(mx, my, button, modkeys);
  }
  
  void Menu::show() {

    if (GUI_DIRECTION_DOWN == direction) { 
      setOverlay(x, y, w, (h + popup_height));
    }
    else {
      setOverlay(x, y - popup_height, w, popup_height);
    }

    Widget::show();
  }

  void Menu::hide() {

    unsetOverlay();
    Widget::hide();
  }

} /* namespace rx */
