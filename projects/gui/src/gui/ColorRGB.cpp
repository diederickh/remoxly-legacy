#include <gui/ColorRGB.h>
#include <gui/Group.h>
#include <gui/Render.h>
#include <gui/Utils.h>

ColorRGB::ColorRGB(std::string label, float* rgb, int ncolors, float sat, float val)
  :Widget(GUI_TYPE_COLOR_RGB, label)
  ,rgb(rgb)
  ,perc_value(0.0f)
  ,color_dx(0)
{
  h = 22;

  float h,s,v = 0.0f;

  for(int i = 0; i < ncolors; ++i) {
    float p = float(i)/(ncolors-1);
    ColorRect rect;
    rect.rgba[3] = 0.7f;
    gui_hsv_to_rgb(p, sat, val,  rect.rgba[0], rect.rgba[1], rect.rgba[2]);
    colors.push_back(rect);
  }
}

void ColorRGB::create() {

  // draw the colored rectangles
  float rect_w = float(w) / colors.size();
  float rect_x = x;

  for(size_t i = 0; i < colors.size(); ++i) {
    render->addRectangle(rect_x, y, rect_w, h, colors[i].rgba, true);
    rect_x += rect_w;
  }

  // draw the selected value.
  ColorRect& curr_rect = colors[color_dx];
  float curr_col[] = { curr_rect.rgba[0], curr_rect.rgba[1], curr_rect.rgba[2], 1.0 } ;

  render->addRectangle(x + w - 22, y, 22, h, curr_col);
  render->writeText(x + group->xindent, y + group->yindent, label, group->label_color);
}

void ColorRGB::onMousePress(float mx, float my, int button, int modkeys) {
  
  if(GUI_IS_INSIDE_WIDGET(this, mx, my)) {
    state |= GUI_STATE_DOWN_INSIDE;
    setMousePositionValue(mx);
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

  if(state & GUI_STATE_DOWN_INSIDE) {
    setMousePositionValue(mx);
  }
}

void ColorRGB::setMousePositionValue(int mx) {

  float pos_v = 0.0f;
  int pos_x = 0;
  
  pos_x = gui_clamp<int>((int)mx, x, (x + (w-22)));
  pos_v = 1.0 - float((x + w - 22) - pos_x) / (w-22);

  setPercentageValue(pos_v);
  needs_redraw = true ;
}

void ColorRGB::setPercentageValue(float p) {

  perc_value = gui_clamp<float>(p, 0.0f, 1.0f);
  color_dx = gui_clamp<size_t>(colors.size() * perc_value, 0, colors.size()-1);

  // set the color.
  ColorRect& r = colors[color_dx];
  rgb[0] = r.rgba[0];
  rgb[1] = r.rgba[1];
  rgb[2] = r.rgba[2];
}
