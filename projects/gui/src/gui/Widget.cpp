#include <gui/Types.h>
#include <gui/Gui.h>
#include <gui/Widget.h>
#include <stdio.h>

uint32_t Widget::id = 0;

Widget::Widget(int type, std::string label)
  :type(type)
  ,state(GUI_STATE_NONE)
  ,mods(GUI_MOD_NONE)
  ,label(label)
  ,gui(NULL)
  ,render(NULL)
  ,x(0)
  ,y(0)
  ,w(0)
  ,h(0)
  ,needs_redraw(true)
  ,mouse_x(0)
  ,mouse_y(0)
  ,mouse_press_x(0)
  ,mouse_press_y(0)
  ,press_x(0)
  ,press_y(0)
{
  bbox[0] = bbox[1] = bbox[2] = bbox[3] = 0;
  id++;
}

Widget::~Widget() {
}

bool Widget::setup(Gui* g) {
  gui = g; 
  render = g->render;
  return true; 
}

void Widget::add(Widget* w, Gui* g) {

  w->setup(g);

  children.push_back(w);
}

bool Widget::needsRedraw() {

  if(needs_redraw) {
    return true;
  }

  return needsRedrawChildren();
}

bool Widget::needsRedrawChildren() {

  for(std::vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it) {
    if((*it)->needsRedraw()) {
      return true;
    }
  }
  return false;
}

void Widget::create() {
}

void Widget::build() {

  if(!isDrawn()) {
    unsetNeedsRedraw();
    return;
  }

  create();

  buildChildren();
 
  unsetNeedsRedraw();
}

void Widget::buildChildren() {

 for(std::vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it) {
   (*it)->build();
  }
}

void Widget::onCharPress(unsigned int key) {
  onCharPressChildren(key);
}

void Widget::onCharPressChildren(unsigned int key) {

  for(std::vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it) {
    (*it)->onCharPress(key);
  }
}

void Widget::onKeyPress(int key, int modkeys) {

  mods |= modkeys;
  onKeyPressChildren(key, modkeys);
}

void Widget::onKeyPressChildren(int key, int modkeys) {

  for(std::vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it) {
    (*it)->onKeyPress(key, modkeys);
  }
}

void Widget::onKeyRelease(int key, int modkeys) {

  mods = modkeys;
  onKeyReleaseChildren(key, modkeys);
}

void Widget::onKeyReleaseChildren(int key, int modkeys) {

  for(std::vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it) {
    (*it)->onKeyRelease(key, modkeys);
  }
}


void Widget::onMousePress(float mx, float my, int button, int modkeys) {

  if(!isDrawn()) {
    return;
  }

  mouse_x = mx;
  mouse_y = my;
  mouse_press_x = mx;
  mouse_press_y = my;
  press_x = x;
  press_y = y;

  Widget::onMousePressChildren(mx, my, button, modkeys);
}

void Widget::onMousePressChildren(float mx, float my, int button, int modkeys) {

  for(std::vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it) {
    (*it)->onMousePress(mx, my, button, modkeys);
  }
}

void Widget::onMousePressOutside(float mx, float my, int button, int modkeys) {

  mouse_x = mx;
  mouse_y = my;
  mouse_press_x = mx;
  mouse_press_y = my;
  press_x = x;
  press_y = y;
  state &= ~GUI_STATE_DOWN_INSIDE;
  
  Widget::onMousePressOutsideChildren(mx, my, button, modkeys);
}

void Widget::onMousePressOutsideChildren(float mx, float my, int button, int modkeys) {
  
  for(std::vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it) {
    (*it)->onMousePressOutside(mx, my, button, modkeys);
  }
}

void Widget::onMouseRelease(float mx, float my, int button, int modkeys) {

  if(!isDrawn()) {
    return;
  }

  mouse_x = mx;
  mouse_y = my;
  mouse_press_x = 0;
  mouse_press_y = 0;

  Widget::onMouseReleaseChildren(mx, my, button, modkeys);
}

void Widget::onMouseReleaseChildren(float mx, float my, int button, int modkeys) {

  for(std::vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it) {
    (*it)->onMouseRelease(mx, my, button, modkeys);
  }
}

void Widget::onMouseReleaseOutside(float mx, float my, int button, int modkeys) {

  mods = GUI_MOD_NONE;
  mouse_x = mx;
  mouse_y = my;
  mouse_press_x = 0;
  mouse_press_y = 0;

  Widget::onMouseReleaseOutsideChildren(mx, my, button, modkeys);
}

void Widget::onMouseReleaseOutsideChildren(float mx, float my, int button, int modkeys) {

  for(std::vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it) {
    (*it)->onMouseReleaseOutside(mx, my, button, modkeys);
  }
}

void Widget::onMouseMove(float mx, float my) {

  Widget::onMouseMoveChildren(mx, my);

  mouse_x = mx;
  mouse_y = my;
}

void Widget::onMouseMoveChildren(float mx, float my) {

  for(std::vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it) {
    (*it)->onMouseMove(mx, my);
  }
}

void Widget::position() {
}

void Widget::setBoundingBox() {
  
  if(!isDrawn()) {
    bbox[0] = 0;
    bbox[1] = 0;
    bbox[2] = 0;
    bbox[3] = 0;
    return;
  }

  setBoundingBoxChildren();

  bbox[0] = x;
  bbox[1] = y;
  bbox[2] = w;
  bbox[3] = h;
  
  for(std::vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it) {

    Widget* wid = *it;
    bbox[3] += wid->bbox[3];

    if(wid->isDrawn()) {
      bbox[3] += gui->padding;
    }
  }
}

void Widget::setBoundingBoxChildren() {

  for(std::vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it) {
    (*it)->setBoundingBox();
  }
}

void Widget::showChildren() {

  for(std::vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it) {
    (*it)->show();
  }
}

void Widget::hideChildren() {

  for(std::vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it) {
    (*it)->hide();
  }
}

void Widget::closeChildren() {

  for(std::vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it) {
    (*it)->close();
  }
}

void Widget::openChildren() {

  for(std::vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it) {
    (*it)->open();
  }
}

void Widget::removeChildren() {

  for(std::vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it) {
    Widget* wid = *it;
    delete wid;
  }

  children.clear();
}

void Widget::lockPosition() {
  state |= GUI_STATE_POSITION_LOCKED;
}

void Widget::unlockPosition() {
  state &= ~GUI_STATE_POSITION_LOCKED;
}

void Widget::show() {
  state &= ~GUI_STATE_HIDDEN;
}

void Widget::open() {
  state &= ~GUI_STATE_CLOSED;
}

void Widget::close() {
  state |= GUI_STATE_CLOSED;
}

void Widget::hide() {

  bbox[0] = 0;
  bbox[1] = 0;
  bbox[2] = 0;
  bbox[3] = 0;

  state |= GUI_STATE_HIDDEN;
}

void Widget::print() {
  printf("bbox.x: %d, bbox.y: %d, bbox.w: %d, bbox.h: %d\n", bbox[0], bbox[1], bbox[2], bbox[3]);
}

void Widget::unsetNeedsRedraw() {
  needs_redraw = false;
}

void Widget::unsetNeedsRedrawChildren() {

  for(std::vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it) {
    (*it)->unsetNeedsRedraw();
  }
}
