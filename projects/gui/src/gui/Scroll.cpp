#include <cmath>
#include <algorithm>
#include <gui/Scroll.h>
#include <gui/Render.h>
#include <gui/Group.h>

namespace rx { 

// -------------------------------------------

void scroll_up_click(int id, void* user) {
  Scroll* scroll =  static_cast<Scroll*>(user);
  scroll->movePageUp();
}

void scroll_down_click(int id, void* user) {
  Scroll* scroll =  static_cast<Scroll*>(user);
  scroll->movePageDown();
}

// -------------------------------------------

Scroll::Scroll()
  :Widget(GUI_TYPE_SCROLL, "")
  ,up_button(0, GUI_ICON_CHEVRON_UP, scroll_up_click, this, GUI_CORNER_TOP)
  ,down_button(1, GUI_ICON_CHEVRON_DOWN, scroll_down_click, this, GUI_CORNER_BOTTOM)
  ,perc(0.0f)
  ,visible_x(0)
  ,visible_y(0)
  ,visible_w(0)
  ,visible_h(0)
  ,content_h(0)
  ,prev_content_h(0)
  ,offset_y(0)
  ,grip_y(0)
  ,grip_h(0)
  ,grip_space(0)
  ,track_y(0)
  ,track_h(0)
  ,scroll_bar_x(0)
  ,scroll_bar_y(0)
{
  w = 22;
}


void Scroll::setGroup(Group* g) {
 
  Widget::setGroup(g);
  down_button.setGroup(g);
  up_button.setGroup(g);
}

void Scroll::create() {
 
  render->addRectangle(visible_x, y, visible_w - group->padding, visible_h - group->padding, group->panel_color, false, 0.0f, 0.0f);  // backround on visible area; e.g. adds the border at the bottom 
  render->addRectangle(scroll_bar_x, scroll_bar_y, w + group->padding, visible_h + group->padding, group->panel_color, 0.0f, 0.0f);  // background scrollbar
  render->addRectangle(scroll_bar_x, track_y, w, track_h, group->getBackgroundStateColor(this, GUI_STATE_DOWN_CUSTOM0), true, 0.0f, 0.0f); // the track 
  render->addRectangle(scroll_bar_x, grip_y, w, grip_h, group->getSelectedStateColor(this, GUI_STATE_DOWN_INSIDE), true, 0.0f, 0.0f); // the grip

  up_button.create();
  down_button.create();
  
  needs_redraw = false;
}

void Scroll::onMousePress(float mx, float my, int button, int modkeys) {
 
  up_button.onMousePress(mx, my, button, modkeys);
  down_button.onMousePress(mx, my, button, modkeys);
  needs_redraw = (up_button.needs_redraw || down_button.needs_redraw);

  // means the user pressed the buttons; so skip press check of grip
  if(needs_redraw) {
    return;
  }
  
  // press inside grip area
  if(GUI_IS_INSIDE(mx, my, scroll_bar_x, grip_y, w, grip_h)) {
    state |= GUI_STATE_DOWN_INSIDE;
    needs_redraw = true;
  }
  // press inside track area
  else if(GUI_IS_INSIDE(mx, my, scroll_bar_x, track_y, w, track_h)) {
    state |= GUI_STATE_DOWN_CUSTOM0;
    needs_redraw = true;
  }
  else {
    if(state & GUI_STATE_DOWN_INSIDE) {
      needs_redraw = true;
      state &= ~GUI_STATE_DOWN_INSIDE;
    }
  }

  Widget::onMousePress(mx, my, button, modkeys);
}

void Scroll::onMouseRelease(float mx, float my, int button, int modkeys) {

  up_button.onMouseRelease(mx, my, button, modkeys);
  down_button.onMouseRelease(mx, my, button, modkeys);
  needs_redraw = (up_button.needs_redraw || down_button.needs_redraw);

  if(state & (GUI_STATE_DOWN_INSIDE | GUI_STATE_DOWN_CUSTOM0)) {
    needs_redraw = true;
  }

  if(state & GUI_STATE_DOWN_CUSTOM0) {
    if(GUI_IS_INSIDE(mx, my, scroll_bar_x, track_y, w, track_h)) {
      if(mouse_press_y <= grip_y) {
        movePageUp();
      }
      else {
        movePageDown();
      }
    }
  }

  state &= ~GUI_STATE_DOWN_INSIDE;
  state &= ~GUI_STATE_DOWN_CUSTOM0;

  Widget::onMouseRelease(mx, my, button, modkeys);
}

void Scroll::onMouseMove(float mx, float my) {
  
  if(state & GUI_STATE_DOWN_INSIDE) {
     setMousePositionValue(my);
  }

  Widget::onMouseMove(mx, my);
}

void Scroll::setMousePositionValue(float my) {

  int offset_mouse = mouse_y - my;
  int new_grip_y = gui_clamp<int>((int)grip_y - offset_mouse, track_y, track_y + grip_space);
  float grip_perc = float(new_grip_y - track_y) / (grip_space);

  setPercentageValue(grip_perc);
  needs_redraw = true;
}

void Scroll::setPercentageValue(float v) {
 
  perc = gui_clamp<float>(v, 0.0f, 1.0f);
  grip_y = track_y + (perc * grip_space);
}

void Scroll::setVisibleArea(int visx, int visy, int visw, int vish, int contenth) {

  prev_content_h = content_h;
  visible_x = visx;
  visible_y = visy;
  visible_w = visw;
  visible_h = vish;
  content_h = contenth;

  int div = prev_content_h - content_h;
  if(div) {
    // @todo - when then contents becomes smaller/bigger we need to adjust the percentage. reproduce: Create a panel with these groups and widgets: https://gist.github.com/roxlu/218d6ab2039e9967cf7c . 1) open all groups 2) scroll down, 3) close water simulation 4) open water simulation. This is now fixed by calling Panel::position() again in Panel::position() when the offsets change.
  }
}

void Scroll::position() {

  up_button.x = visible_x + visible_w;
  up_button.y = visible_y + group->padding;
  down_button.x = visible_x + visible_w;
  down_button.y = visible_y + visible_h - (down_button.h + group->padding);

  // scrollbar, track and grip size
  scroll_bar_x = visible_x + visible_w;
  scroll_bar_y = y - group->padding;
  track_y = scroll_bar_y + up_button.h + group->padding * 2;
  track_h = visible_h - (down_button.h + up_button.h + group->padding * 4);
 
  float content_ratio = float(visible_h) / content_h;
  grip_h = gui_clamp<int>(content_ratio * track_h, 10, track_h * 0.9);
  grip_space = track_h - grip_h; 
 
  // how much does the content needs to be offset
  offset_y = std::max<int>((content_h - visible_h) * perc, 0);
 
  setPercentageValue(perc);
}

void Scroll::movePageUp() {
 
  float page_ratio = float(visible_h) / content_h;
  setPercentageValue(perc - page_ratio);
  position();
  needs_redraw = true;
}

void Scroll::movePageDown() {

  float page_ratio = float(visible_h) / content_h;
  setPercentageValue(perc + page_ratio);
  position();
  needs_redraw = true;
}

void Scroll::beginScissor() {

  int win_w;
  int win_h;

  render->getWindowSize(win_w, win_h);

  render->beginScissor();

  render->scissor(visible_x, 
                  win_h - (visible_h + visible_y), 
                  visible_w + w + group->padding,
                  visible_h);
}

void Scroll::endScissor() {
  render->endScissor();
}

} // namespace rx 
