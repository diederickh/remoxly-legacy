#include <gui/Menu.h>
#include <gui/Group.h>
#include <gui/Render.h>

namespace rx {

  Menu::Menu(std::string title, std::vector<std::string> options)
    :Widget(GUI_TYPE_MENU, title)
    ,options(options)
    ,selected_dx(-1)
    ,draw_dx(-1)
  {
    h = 22;
    popup_height = options.size() * h;
  }

  void Menu::create() {

    int yy = y + h;
    size_t last = options.size() - 1;
    
    render->setLayer(1);

    //    render->addRoundedRectangle(x, yy, w, popup_height, 6.0, group->getSelectedStateColor(this), true, GUI_CORNER_BOTTOM);
    render->addRoundedRectangle(x, yy, w, popup_height, 6.0, group->header_color, true, GUI_CORNER_BOTTOM);

    for (size_t i = 0; i < options.size(); ++i) {
      if (GUI_IS_INSIDE(mouse_x, mouse_y, (x + group->xindent), (yy + group->yindent), w, h)) {
        render->addRoundedRectangle(x, yy, w, h, 6.0, group->selected_color, true, (i == last) ? GUI_CORNER_BOTTOM : GUI_CORNER_NONE);
        render->writeText(x + group->xindent, yy + group->yindent, options[i], group->label_color);
        draw_dx = (int32_t)i;
      }
      else {
        render->writeText(x + group->xindent, yy + group->yindent, options[i], group->number_color);
      }

      yy += h;
    }

    render->setLayer(0);
    setDepth(100.0f);
  }


  void Menu::onMouseMove(float mx, float my) {

    Widget::onMouseMove(mx, my);

    if (state & GUI_STATE_HIDDEN) {
      return;
    }

    if (GUI_IS_INSIDE(mx, my, x, y, w, (h + popup_height))) {
      needs_redraw = true;
    }
  }

  void Menu::onMouseRelease(float mx, float my, int button, int modkeys) {

    if (GUI_IS_INSIDE(mx, my, x, y+h, w, (popup_height))) {
      selected_dx = draw_dx;
    }

    Widget::onMouseRelease(mx, my, button, modkeys);
  }


} /* namespace rx */
