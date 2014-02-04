#include <assert.h>
#include <gui/IconButton.h>
#include <gui/Render.h>
#include <gui/Gui.h>

IconButton::IconButton(int callid, unsigned int icon, gui_button_callback cb, void* user)
  :Widget(GUI_TYPE_ICON_BUTTON, "")
  ,cb_id(callid)
  ,cb_click(cb)
  ,cb_user(user)
  ,icon(icon)
  ,icon_w(0)
  ,icon_h(0)
  ,icon_x(0)
  ,icon_y(0)
{
  w = 22;
  h = 22;
}

void IconButton::setGui(Gui* g) {

  Widget::setGui(g);
  render->getIconSize(icon, icon_w, icon_h);
}

void IconButton::create() {

  assert(gui && render);

  if(state & GUI_STATE_HIDDEN) {
    return;
  }

  int bt_click_offset = 0;

  if(state & GUI_STATE_DOWN_INSIDE) {
    bt_click_offset = 1;
  }

  int cx = x + (w * 0.5 ) - (icon_w * 0.5) + icon_x;
  int cy = y + (h * 0.5 ) - (icon_h * 0.5) + icon_y;

  render->addRectangle(x, y, w, h, gui->getButtonStateColor(this), true);
  render->writeIcon(cx, cy + bt_click_offset, icon, gui->label_color);

  needs_redraw = false;
}

void IconButton::onMouseRelease(float mx, float my, int button, int mods) {

  if(!isDrawn()) {
    return;
  }

  if(GUI_IS_INSIDE_WIDGET(this, mx, my) && (state & GUI_STATE_DOWN_INSIDE)) {

    call();

    state &= ~GUI_STATE_DOWN_INSIDE;
    needs_redraw = true;
  }
  else {
    if(state & GUI_STATE_DOWN_INSIDE) {
      needs_redraw = true;
    }
    state &= ~GUI_STATE_DOWN_INSIDE;
  }
}

void IconButton::onMousePress(float mx, float my, int button, int mods) {

  if(!isDrawn()) {
    return;
  }

  if(GUI_IS_INSIDE_WIDGET(this, mx, my)) {
    state |= GUI_STATE_DOWN_INSIDE;
    needs_redraw = true;
  }
  else {
    state &= ~GUI_STATE_DOWN_INSIDE;
  }
}

void IconButton::call() {

  if(cb_click) {
    cb_click(cb_id, cb_user);
  }
}
