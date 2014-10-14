#include <gui/Text.h>
#include <gui/Render.h>
#include <gui/Group.h>

namespace rx { 

Text::Text(std::string label, std::string& value, int textw)
  :Widget(GUI_TYPE_TEXT, label)
  ,value(value)
  ,text_x(0)
  ,text_w(textw)
{
  h = GUI_WIDGET_DEFAULT_HEIGHT;
}

void Text::create() {

  int label_w = 0;
  label_w = w - text_w;
  text_x = x + label_w;

  render->addRectangle(x, y, label_w - group->padding, h, group->bg_color, true, 0.0, 0.1f);
  render->addRectangle(text_x, y, text_w,  h, group->text_bg_color, true, -0.1f, 0.0f);
  render->writeText(x + group->xindent, y + group->yindent, label, group->label_color);

  if(state & GUI_STATE_EDITABLE) {
    render->enableTextInput(text_x + group->xindent, y + group->yindent, text_w, value, group->label_color);
  }
  else {
    render->writeText(text_x + group->xindent, y + group->yindent, value, group->label_color);
  }
}

void Text::onCharPress(unsigned int key) {

  if(state & GUI_STATE_EDITABLE) {
    render->onCharPress(key);
  }
}

void Text::onKeyPress(int key, int modkeys) {
  if(state & GUI_STATE_EDITABLE) {
	  switch( key )
	  {
	  case GUI_KEY_ENTER:
	  case GUI_NATIVE_KEY_ENTER:
	      disableInputAndCopyValue();
		  break;
	  default:
	      render->onKeyPress(key, modkeys);
		  break;
	  }
  }
}

void Text::onMousePress(float mx, float my, int button, int modkeys) {

  // text input
  if(GUI_IS_INSIDE(mx, my, text_x, y, text_w, h)) {
    if(!(state & GUI_STATE_EDITABLE)) {
      state |= GUI_STATE_EDITABLE;
      needs_redraw = true;
    }
  }
  else {
    if(state & GUI_STATE_EDITABLE) {
      disableInputAndCopyValue();
    }
  }
}

void Text::onMousePressOutside(float mx, float my, int button, int modkeys) {

  if(state & GUI_STATE_EDITABLE) {
    disableInputAndCopyValue();
  }
}

void Text::disableInputAndCopyValue() {
  
  render->getTextInputValue(value);

  render->disableTextInput();

  state &= ~GUI_STATE_EDITABLE;
  needs_redraw = true;

  notify(GUI_EVENT_VALUE_CHANGED);
}

} // namespace rx
