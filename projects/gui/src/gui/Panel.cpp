#include <gui/Panel.h>
#include <gui/Scroll.h>
#include <gui/Render.h>
#include <gui/Gui.h>
#include <stdio.h>

Panel::Panel(Render* r, int height)
  :Widget(GUI_TYPE_PANEL, "")
{
  x = 10;
  y = 10;
  w = 275;
  h = height;
  render = r;
  scroll.render = r;
}

Gui* Panel::addGui(std::string title) {

  Gui* g = new Gui(title, render);

  if(!g->setup()) {
    printf("Cannot setup the gui.\n");
    return NULL;
  }

  g->lockPosition();
 
  if(!guis.size()) {
    scroll.setup(g);
    gui = g;
  }

  add(g, g);

  guis.push_back(g);

  return g;
}

void Panel::position() {

  int start_offset_y = scroll.offset_y;
  int gx = x;
  int gy = y - scroll.offset_y;
  int gw = w;
  int gh = h;
  int content_h = 0;

  for(std::vector<Gui*>::iterator it = guis.begin(); it != guis.end(); ++it) {

    Gui* g = *it;

    g->x = gx;
    g->y = gy;
    g->w = gw;
    
    g->position();

    gy += g->bbox[3] + g->padding;
    content_h += g->bbox[3] + g->padding;
  }

  content_h += gui->padding; 

  scroll.x = x;
  scroll.y = y;
  scroll.setVisibleArea(x - gui->padding, y - gui->padding, gw + gui->padding * 2, h, content_h); 
  scroll.position();

  // @todo - we need a better fix for this. when the content height changes, i.e. becomes smaller, then we need to reposition; this is kind of a hack which works okay, but is not perfect. 
  if(start_offset_y != scroll.offset_y) {
    position();
  }
}

void Panel::create() {
  
  render->addRectangle(x - gui->padding, y, w + gui->padding * 2, h, gui->panel_color); // background
  needs_redraw = false;
}

void Panel::update() {

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

  update();

  scroll.beginScissor();
    render->draw();
  scroll.endScissor();
}

void Panel::onMousePress(float mx, float my, int button, int modkeys) {

  if(GUI_IS_INSIDE(mx, my, scroll.visible_x, scroll.visible_y, scroll.visible_w, scroll.visible_h)) {
    Widget::onMousePress(mx, my, button, modkeys);

    // check if we pressed inside a header of gui to allow dragging
    for(std::vector<Gui*>::iterator it = guis.begin(); it != guis.end(); ++it) {
      Gui* g = *it;
      if(GUI_IS_INSIDE(mx, my, g->x, g->y, (g->w - g->close_button.w) , g->h)) {
        state |= GUI_STATE_DOWN_INSIDE;
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
