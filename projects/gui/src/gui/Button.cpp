#include <gui/Button.h>
#include <gui/Group.h>
#include <gui/Render.h>

Button::Button(std::string title, int id, unsigned int icon, gui_button_callback cb, void* user, int iconx, int icony)
  :Widget(GUI_TYPE_BUTTON, title)
  ,icon_button(id, icon, cb, user)
{
  h = 22;

  if(iconx) {
    icon_button.icon_x = iconx;
  }

  if(icony) {
    icon_button.icon_y = icony;
  }
}

void Button::setGroup(Group* g) {

  Widget::setGroup(g);
  add(&icon_button, g);
}

void Button::create() {

  int bt_click_offset = 0;
  if(state & GUI_STATE_DOWN_INSIDE) {
    bt_click_offset = 1;
  }

  render->addRectangle(x, y, w, h, group->getButtonStateColor(this), true);
  render->writeText(x + group->xindent, y + group->yindent + bt_click_offset, label, group->label_color);
}

void Button::position() {

  icon_button.x = x + (w - icon_button.w);
  icon_button.y = y;
}

// because we added the icon_button as a child it will be 
// added to the bounding box by default; we don't want that
void Button::setBoundingBox() {

  if(!isDrawn()) {
    bbox[0] = bbox[1] = bbox[2] = bbox[3] = 0;
    return;
  }

  bbox[0] = x; 
  bbox[1] = y;
  bbox[2] = w;
  bbox[3] = h;
}

void Button::onMousePress(float mx, float my, int button, int modkeys) {
  
  Widget::onMousePress(mx, my, button, modkeys);

  if(GUI_IS_INSIDE_WIDGET(this, mx, my)) {
    state |= GUI_STATE_DOWN_INSIDE;
    icon_button.state |= GUI_STATE_DOWN_INSIDE;
    needs_redraw = true;
  }
}

void Button::onMouseRelease(float mx, float my, int button, int modkeys) {
  
  if(state & GUI_STATE_DOWN_INSIDE) {
    icon_button.call();
    needs_redraw = true;
  }

  state &= ~GUI_STATE_DOWN_INSIDE;
  icon_button.state &= ~GUI_STATE_DOWN_INSIDE;
  Widget::onMouseRelease(mx, my, button, modkeys);
}
