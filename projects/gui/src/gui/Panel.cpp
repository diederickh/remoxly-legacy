#include <gui/Panel.h>
#include <gui/Scroll.h>
#include <gui/Render.h>
#include <gui/Group.h>
#include <stdio.h>

namespace rx { 

Panel::Panel(Render* r, int height, int style)
  :Widget(GUI_TYPE_PANEL, "")
  ,scroll(style)
{
  x = 10;
  y = 10;
  w = 275;
  h = height;
  render = r;
  scroll.render = r;
}

Panel::~Panel() {

  for(std::vector<Group*>::iterator it = groups.begin(); it != groups.end(); ++it) {
    Group* g = *it;
    delete g;
    g = NULL;
  }

  groups.clear();
}

Group* Panel::addGroup(std::string title, int style) {

  Group* g = new Group(title, render, style);
  
  g->lockPosition();
 
  if(!groups.size()) {
    scroll.setGroup(g);
    group = g;
  }

  add(g, g);

  groups.push_back(g);

  return g;
}

void Panel::position() {

  int start_offset_y = scroll.offset_y;
  int gx = x;
  int gy = y - scroll.offset_y;
  int gw = w;
  int gh = h;
  int content_h = 0;

  for(std::vector<Group*>::iterator it = groups.begin(); it != groups.end(); ++it) {

    Group* g = *it;

    g->x = gx;
    g->y = gy;
    g->w = gw;
    
    g->position();
    gy += g->bbox[3] + g->padding;
    content_h += g->bbox[3] + g->padding;
  }

  if (NULL != group) {

    content_h += group->padding; 

    scroll.x = x;
    scroll.y = y;
    scroll.setVisibleArea(x - group->padding, y - group->padding, gw + group->padding * 2, h, content_h); 
    scroll.position();
  }

  // @todo - we need a better fix for this. when the content height changes, i.e. becomes smaller, then we need to reposition; this is kind of a hack which works okay, but is not perfect. 
  if(start_offset_y != scroll.offset_y) {
    position();
  }
}

void Panel::create() {
  
  if (NULL != group) {
    render->addRectangle(x - group->padding, y, w + group->padding * 2, h, group->panel_color); // background
    needs_redraw = false;
  }
}

void Panel::update() {

  if (0 == groups.size()) {
    return;
  }

  if(needsRedraw()) {
    render->clear();
    position();
    build();
    scroll.create();
    render->update();
    needs_redraw = false;
  }
}

void Panel::draw() {

  if (0 == groups.size()) {
    return;
  }

  update();

  scroll.beginScissor();
    render->draw();
  scroll.endScissor();
}

void Panel::onMousePress(float mx, float my, int button, int modkeys) {

  if(GUI_IS_INSIDE(mx, my, scroll.visible_x, scroll.visible_y, scroll.visible_w, scroll.visible_h)) {
    Widget::onMousePress(mx, my, button, modkeys);

    // check if we pressed inside a header of group to allow dragging
    if (false == (state & GUI_STATE_POSITION_LOCKED)) {
      for(std::vector<Group*>::iterator it = groups.begin(); it != groups.end(); ++it) {
        Group* g = *it;
        if(GUI_IS_INSIDE(mx, my, g->x, g->y, (g->w - g->close_button.w) , g->h)) {
          state |= GUI_STATE_DOWN_INSIDE;
        }
      }
    }
  }
  else {
    Widget::onMousePressOutside(mx, my, button, modkeys);
  }

  scroll.onMousePress(mx, my, button, modkeys);
}

void Panel::onMouseRelease(float mx, float my, int button, int modkeys) {

  state &= ~GUI_STATE_DOWN_INSIDE;

  Widget::onMouseRelease(mx, my, button, modkeys);
  scroll.onMouseRelease(mx, my, button, modkeys);
}

void Panel::onMouseMove(float mx, float my) {

  if(state & GUI_STATE_DOWN_INSIDE) {
    int dx = press_x + (mx - mouse_press_x);
    int dy = press_y + (my - mouse_press_y);
    x = dx;
    y = dy;
    needs_redraw = true;
  }

  Widget::onMouseMove(mx, my);
  scroll.onMouseMove(mx, my);
}

bool Panel::needsRedraw() {
  return needs_redraw || scroll.needs_redraw || needsRedrawChildren();
}

} // namespace rx
