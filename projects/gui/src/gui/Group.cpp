#include <gui/Utils.h>
#include <gui/Group.h>

namespace rx { 

  /* -------------------------------------------------------------------------------------------------------------- */

  void group_close_click(int id, void* user) {

    Group* g = static_cast<Group*>(user);
    g->closeChildren();
    g->close_button.hide();
    g->open_button.show();
    g->needs_redraw = true;
  }

  void group_open_click(int id, void* user) {

    Group* g = static_cast<Group*>(user);
    g->openChildren();
    g->close_button.show();
    g->open_button.hide();
    g->needs_redraw = true;
  }

  /* -------------------------------------------------------------------------------------------------------------- */

  Group::Group(std::string label, Render* r) 
    :Widget(GUI_TYPE_GROUP, label)
    ,render(r)
    ,show_header(true)
    ,padding(1)
    ,xindent(7)
    ,yindent(6)
    ,close_button(0, GUI_ICON_CARET_DOWN, group_close_click, this, GUI_CORNER_RIGHT)
    ,open_button(0, GUI_ICON_CARET_RIGHT, group_open_click, this, GUI_CORNER_RIGHT)
    ,shade_top(0.1f)
    ,shade_bottom(-0.1f)
  {
    x = 10;
    y = 10;
    w = 275;
    h = 22;
  
    setGroup(this);

    /* Initialize with some default colors @todo, we could simply put this colors directly in setColors. */
    float theme_fg_color[] = { 0.6, 0.6, 0.6, 1.0};
    float theme_bg_color[] = { 0.5351, 0.5351, 0.5351, 1.0 };
    float theme_hl_color[] = { 0.394, 0.0f, 0.917, 1.0 };
    setColors(theme_bg_color, theme_fg_color, theme_hl_color);

    close_button.setGroup(this);
    open_button.setGroup(this);
    open_button.hide();
  }

  Group::~Group() {
    removeChildren();
  }

  void Group::setColors(float* bg, float* fg, float* highlight) {
    gui_fill_color(0.0f, 0.0f, 0.0f, 1.0f, label_color);
    gui_fill_color(1.0f, 1.0f, 1.0f, 1.0f, number_color);
    gui_fill_color(bg[0], bg[1], bg[2], bg[3], bg_color);
    gui_fill_color(fg[0], fg[1], fg[2], fg[3], fg_color);
    gui_fill_color(fg[0], fg[1], fg[2], fg[3], button_color);
    gui_fill_color(highlight[0], highlight[1], highlight[2], highlight[3], highlight_color);
    gui_fill_color(0.337, 0.502, 0.761, 1.0, selected_color); /* blue */
    gui_fill_color(0.247, 0.257, 0.247, 1.0, header_color);   /* gray */
    gui_fill_color(0.706, 0.706, 0.706, 1.0, text_bg_color);  /* light gray */
  }

  float* Group::getStateColor(Widget* w, int flag, float* off, float* on) {

    if(w->state & flag) {
      return on;
    }
    else {
      return off;
    }
  }

  float* Group::getButtonStateColor(Widget* w, int flag) {
    return getStateColor(w, flag, button_color, highlight_color);
  }

  float* Group::getForegroundStateColor(Widget* w, int flag) {
    return getStateColor(w, flag, fg_color, highlight_color);
  }

  float* Group::getBackgroundStateColor(Widget* w, int flag) {
    return getStateColor(w, flag, bg_color, highlight_color);
  }

  float* Group::getSelectedStateColor(Widget* w, int flag) {
    return getStateColor(w, flag, selected_color, highlight_color);
  }

  void Group::add(Widget* wid) {
  
    if(!group) {
      printf("Error: first call setup() on the Group before adding elements.\n");
      return;
    }

    Widget::add(wid, this);
  }

  void Group::create() {

    if (false == show_header) {
      return;
    }

    render->addRectangle(x - padding, y - padding, w + padding * 2, bbox[3] + padding * 2, panel_color, true);
    render->addRoundedRectangle(x, y, w - close_button.w, h, 6, header_color, true, shade_top, shade_bottom, GUI_CORNER_NONE);
    render->writeText(x + xindent, y + yindent, label, number_color);

    open_button.create();
    close_button.create();

    needs_redraw = false;
  }

  void Group::draw() {

    update();

    render->draw();
  }

  void Group::update() {

    if(needsRedraw()) {
      render->clear();
      position();
      build();
      render->update();
    }
  }

  void Group::position() {

    int curr_x = x;
    int curr_y = y + h + padding;
    int el_w = 0;
    int el_h = 0;
    int count = 0;

    for(std::vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it) {
    
      Widget* wid = *it;

      if(wid->state & GUI_STATE_HIDDEN) {
        continue;
      }

      wid->x = curr_x;
      wid->y = curr_y + (count * padding);
      wid->w = w;

      wid->position();
      wid->setBoundingBox();

      curr_y += wid->bbox[3];

      ++count;
    }

    close_button.x = (x + w) - (close_button.w);
    close_button.y = y;
    open_button.x = close_button.x;
    open_button.y = close_button.y; 
  
    setBoundingBox();
  }

  void Group::onMousePress(float mx, float my, int button, int modkeys) {

    Widget::onMousePress(mx, my, button, modkeys);

    if(!(state & GUI_STATE_POSITION_LOCKED) && GUI_IS_INSIDE(mx, my, x, y, w, h)) {
      state |= GUI_STATE_DOWN_INSIDE;
    }

    close_button.onMousePress(mx, my, button, modkeys);
    open_button.onMousePress(mx, my, button, modkeys);
  }

  void Group::onMouseRelease(float mx, float my, int button, int modkeys) {
  
    Widget::onMouseRelease(mx, my, button, modkeys);

    state &= ~GUI_STATE_DOWN_INSIDE;

    close_button.onMouseRelease(mx, my, button, modkeys);
    open_button.onMouseRelease(mx, my, button, modkeys);
  }

  void Group::onMouseMove(float mx, float my) {
  
    /* mouse down in header */
    if(state & GUI_STATE_DOWN_INSIDE) {
      int ddx = press_x + (mx - mouse_press_x);
      int ddy = press_y + (my - mouse_press_y);
      x = ddx;
      y = ddy;
      needs_redraw = true;
    }

    Widget::onMouseMove(mx, my);
  }

  bool Group::needsRedraw() {
    return Widget::needsRedraw() || close_button.needs_redraw || open_button.needs_redraw;
  }

  void Group::unsetNeedsRedraw() {
    needs_redraw = false;
    close_button.needs_redraw = false;
    open_button.needs_redraw = false;
  }

} /* namespace rx */
