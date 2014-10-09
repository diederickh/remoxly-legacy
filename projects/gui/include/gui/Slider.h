/*

  Slider
  ------

  A basic slider for number values. 

 */
#ifndef ROXLU_GUI_SLIDER_H
#define ROXLU_GUI_SLIDER_H

#include <string>
#include <cmath>
#include <gui/Group.h>
#include <gui/Render.h>
#include <gui/IconButton.h>
#include <gui/Utils.h>

namespace rx { 

// -------------------------------------------

static void slider_min_click(int id, void* user);
static void slider_plus_click(int id, void* user);

// -------------------------------------------

template<class T>
class Slider : public Widget {

 public:
  Slider(std::string label, T& value, T minv, T maxv, T step);
  void setGroup(Group* g);
  void create();
  void position();
  bool needsRedraw();

  void unsetNeedsRedraw();

  /* interaction */
  void onCharPress(unsigned int key);
  void onKeyPress(int key, int modkeys);
  void onMousePress(float mx, float my, int button, int modkeys);
  void onMousePressOutside(float mx, float my, int button, int modkeys);
  void onMouseRelease(float mx, float my, int button, int modkeys);
  void onMouseReleaseOutside(float mx, float my, int button, int modkeys);  
  void onMouseMove(float mx, float my);
  
  /* slider specific */
  void setAbsoluteValue(T v);                                       /* set the value to the given one, adjusting the slider is necessary */
  void setPercentageValue(float p);                                 /* set the value to this percentage, 0.0f = minv, 1.0f = maxv */
  void setMousePositionValue(float mx);                             /* set the value based on the given mouse position; we use text_x and text_w to calculate the relative offset of the mouse and use that as percentage */
  void disableInputAndCopyValue();
  void stepMin();
  void stepPlus();

  void setType(int dummy);
  void setType(float dummy);

 public:
  T& value;                                                         /* -the- value */
  T minv;                                                           /* the minimum value for the slider */
  T maxv;                                                           /* the maximum value for the slider */
  T step;                                                           /* when one of the buttons is click this value is subtracted or added to the value */
  float perc_value;                                                 /* the percentage which represents the current value relative to minv and maxv, is used to draw the slided value */
  int num_x;                                                        /* x position where we start drawing the number */
  int num_w;                                                        /* the width of the number field */
  int text_x;
  int text_w;
  IconButton min_button;                                            /* button that decreases the value by step size */
  IconButton plus_button;
};

// -------------------------------------------

static void slider_min_click(int id, void* user) {

  Widget* w = static_cast<Widget*>(user);

  if(w->type == GUI_TYPE_SLIDER_INT) {
    Slider<int>* s = static_cast<Slider<int>* >(user);
    s->stepMin();
  }
  else {
    Slider<float>* s = static_cast<Slider<float>* >(user);
    s->stepMin();
  }
}

static void slider_plus_click(int id, void* user) {

  Widget* w = static_cast<Widget*>(user);

  if(w->type == GUI_TYPE_SLIDER_INT) {
    Slider<int>* s = static_cast<Slider<int>* >(user);
    s->stepPlus();
  }
  else {
    Slider<float>* s = static_cast<Slider<float>* >(user);
    s->stepPlus();
  }
}

// -------------------------------------------

template<class T>
Slider<T>::Slider(std::string label, T& value, T minv, T maxv, T step)
  :Widget(GUI_TYPE_SLIDER_INT, label)
  ,value(value)
  ,minv(minv)
  ,maxv(maxv)
  ,step(step)
  ,perc_value(0.0f)
  ,num_x(0)
  ,num_w(0)
  ,text_x(0)
  ,text_w(0)
  ,min_button(0, GUI_ICON_CHEVRON_LEFT, slider_min_click, this, GUI_CORNER_LEFT)
  ,plus_button(1, GUI_ICON_CHEVRON_RIGHT, slider_plus_click, this, GUI_CORNER_RIGHT)
{
  setType(T(0));
  //setAbsoluteValue(value);
}

template<class T>
void Slider<T>::setGroup(Group* g) {

  Widget::setGroup(g);

  needs_redraw = true;
  h = 22;

  min_button.icon_y = 1;
  plus_button.icon_y = 1;
  plus_button.icon_x = 1;

  min_button.setGroup(g);
  plus_button.setGroup(g);

  /* @todo experimental */
  //  add(&min_button, g);
  //  add(&plus_button, g);
  /* @todo end experimental */

}

template<class T>
void Slider<T>::position() {

  Widget::position();

  plus_button.y = y;
  plus_button.x = x + w - plus_button.w;
  min_button.x = plus_button.x - min_button.w - group->padding;
  min_button.y = y;
}

template<class T>
void Slider<T>::create() {
  
  num_w = 75;
  num_x = min_button.x - num_w - group->padding;
  text_w = w - (min_button.w + plus_button.w + num_w + 3 * group->padding);
  text_x = x;

  render->addRectangle(x, y, text_w, h, group->bg_color, true, 0.0f, 0.1f);
  render->addRectangle(num_x, y, num_w, h, group->text_bg_color, true, -0.1f, 0.0f);
  render->addRectangle(text_x, y, text_w * perc_value, h, group->getSelectedStateColor(this), true, group->shade_top, group->shade_bottom); 
  render->writeText(x + group->xindent, y + group->yindent, label, group->label_color);

  if(state & GUI_STATE_EDITABLE) {
    render->enableNumberInput(num_x + (num_w - group->xindent - group->padding - GUI_CURSOR_WIDTH), y + group->yindent, num_w - 15, value, group->number_color);
  }
  else {
    render->writeNumber(num_x + (num_w - group->xindent - group->padding - GUI_CURSOR_WIDTH), y + group->yindent, value, group->label_color);
  }

  min_button.create();
  plus_button.create();
}

template<class T>
void Slider<T>::onCharPress(unsigned int key) {

  if(state & GUI_STATE_EDITABLE) {
    render->onCharPress(key);
  }
}

template<class T>
void Slider<T>::onKeyPress(int key, int modkeys) {
  
  Widget::onKeyPress(key, modkeys);

  if(state & GUI_STATE_EDITABLE) {
    if(key == GUI_KEY_ENTER) {
      disableInputAndCopyValue();
    }
    else {
      render->onKeyPress(key, modkeys);
    }
  }
}

template<class T>
void Slider<T>::onMousePress(float mx, float my, int button, int modkeys) {
  
  Widget::onMousePress(mx, my, button, modkeys);

  // number:
  if(GUI_IS_INSIDE(mx, my, num_x, y, num_w, h)) {

    // inside number area and not yet editable
    if(!(state & GUI_STATE_EDITABLE)) {
      state |= GUI_STATE_EDITABLE;
      needs_redraw = true;
    }
  }
  else {

    // clicked outside and we were editable, so disable
    if(state & GUI_STATE_EDITABLE) {
      disableInputAndCopyValue();
    }
  }

  // slider:
  if(GUI_IS_INSIDE(mx, my, text_x, y, text_w, h)) {
    setMousePositionValue(mx);
    state |= GUI_STATE_DOWN_INSIDE;
  }

  // pass press onto the buttons.
  min_button.onMousePress(mx, my, button, modkeys);
  plus_button.onMousePress(mx, my, button, modkeys);
}

template<class T>
void Slider<T>::onMouseRelease(float mx, float my, int button, int modkeys) {

  Widget::onMouseRelease(mx, my, button, modkeys);
  min_button.onMouseRelease(mx, my, button, modkeys);
  plus_button.onMouseRelease(mx, my, button, modkeys);

  if(state & GUI_STATE_DOWN_INSIDE) {
    needs_redraw = true;
  }

  state &= ~GUI_STATE_DOWN_INSIDE;
}

template<class T>
void Slider<T>::onMouseReleaseOutside(float mx, float my, int button, int modkeys) {

  if(state & GUI_STATE_EDITABLE) {
    disableInputAndCopyValue();
  }
}

template<class T>
void Slider<T>::onMousePressOutside(float mx, float my, int button, int modkeys) {

  if(state & GUI_STATE_EDITABLE) {
    disableInputAndCopyValue();
  }
}

template<class T>
void Slider<T>::onMouseMove(float mx, float my) {
  
  if(state & GUI_STATE_DOWN_INSIDE) {
    setMousePositionValue(mx);
  }

  Widget::onMouseMove(mx, my);
}

template<class T>
void Slider<T>::setMousePositionValue(float mx) {

  // @todo - in Slider, we need to softly change the values when shift or right mouse button is pressed.
  float slide_v = 0.0f;
  int slide_x = 0;
  
  slide_x = gui_clamp<int>((int)mx, text_x, (text_x + text_w));
  slide_v = float(slide_x - text_x) / text_w;

  //setAbsoluteValue( minv + slide_v * (maxv - minv) );
  setPercentageValue(slide_v);
  needs_redraw = true ;
}

template<class T>
void Slider<T>::disableInputAndCopyValue() {

  value = T(0);
  render->getNumberInputValue(value);
  render->disableNumberInput();

  setAbsoluteValue(value);

  state &= ~GUI_STATE_EDITABLE;
  needs_redraw = true;
}

template<class T>
bool Slider<T>::needsRedraw() {
  return needs_redraw || min_button.needs_redraw || plus_button.needs_redraw;
}

template<class T>
void Slider<T>::stepMin() {

  value -= step;
  setAbsoluteValue(value);
}

template<class T>
void Slider<T>::stepPlus() {

  value += step;
  setAbsoluteValue(value);
}

template<class T> 
void Slider<T>::setType(int dummy) {
  type = GUI_TYPE_SLIDER_INT;
}

template<class T> 
void Slider<T>::setType(float dummy) {
  type = GUI_TYPE_SLIDER_FLOAT;
}

template<class T>
void Slider<T>::setAbsoluteValue(T v) {

  value = v;
  setPercentageValue(float(value)/maxv);
}

// whenever a value changes, this function will be called
template<class T>
void Slider<T>::setPercentageValue(float p) {

	T range = (maxv - minv);
	perc_value = gui_clamp<float>(p, 0.0f, 1.0f);
	value = minv + range * perc_value ;

	float tmp = float(value);
	value = (T)(floorf((tmp/step)) * step);

	// Make sure we stay between min and max
	value = gui_clamp( value, minv, maxv );

	notify(GUI_EVENT_VALUE_CHANGED);
}

template<class T>
void Slider<T>::unsetNeedsRedraw() {
  needs_redraw = false;
  min_button.needs_redraw = false;
  plus_button.needs_redraw = false;
}

} // namespace rx
#endif
