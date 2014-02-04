#include <gui/Group.h>
#include <gui/Render.h>
#include <gui/Toggle.h>

Toggle::Toggle(std::string label, bool& value)
  :Widget(GUI_TYPE_TOGGLE, label)
  ,value(value)
{
  h = 22;
}

void Toggle::create() {

  int bt_click_offset = 0;

  if(state & GUI_STATE_DOWN_INSIDE) {
    bt_click_offset = 1;
  }

  render->addRectangle(x, y, w, h, group->getBackgroundStateColor(this), true);

  if(value) {
    render->writeIcon(x + group->xindent, y + group->yindent + bt_click_offset, GUI_ICON_CHECK_CIRCLE, group->highlight_color);
  }
  else {
    render->writeIcon(x + group->xindent, y + group->yindent + bt_click_offset, GUI_ICON_TIMES_CIRCLE, group->label_color);
  }

  render->writeText(x + group->xindent + 20, y + group->yindent + bt_click_offset, label, group->label_color);
}


void Toggle::onMousePress(float mx, float my, int button, int modkeys) {

  if(GUI_IS_INSIDE_WIDGET(this, mx, my)) {
    state |= GUI_STATE_DOWN_INSIDE;
    needs_redraw = true;
  }
}

void Toggle::onMouseRelease(float mx, float my, int button, int modkeys) {
  
  if(state & GUI_STATE_DOWN_INSIDE) {
    if(GUI_IS_INSIDE_WIDGET(this, mx, my)) {
      value = !value;
    }
    needs_redraw = true;
  }

  state &= ~GUI_STATE_DOWN_INSIDE;
}
