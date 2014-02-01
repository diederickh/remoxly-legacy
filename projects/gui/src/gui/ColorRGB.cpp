#include <gui/ColorRGB.h>
#include <gui/Gui.h>
#include <gui/Render.h>

ColorRGB::ColorRGB(std::string label, float* rgb)
  :Widget(GUI_TYPE_COLOR_RGB, label)
  ,rgb(rgb)
{
  h = 22;
}

void ColorRGB::create() {
  render->addRectangle(x, y, w, h, gui->getBackgroundStateColor(this), true);
  render->writeText(x + gui->xindent, y + gui->yindent, label, gui->label_color);
}

void ColorRGB::onMousePress(float mx, float my, int button, int modkeys) {
  
  if(GUI_IS_INSIDE_WIDGET(this, mx, my)) {
    state |= GUI_STATE_DOWN_INSIDE;
    needs_redraw = true;
  }
}

void ColorRGB::onMouseRelease(float mx, float my, int button, int modkeys) {
  
  if(state & GUI_STATE_DOWN_INSIDE) {
    needs_redraw = true;
  }

  state &= ~GUI_STATE_DOWN_INSIDE;
}

void ColorRGB::onMouseMove(float mx, float my) {
}
