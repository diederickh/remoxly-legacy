/*

  Render
  ------

  This is the interface each of the Render classes needs to 
  implement. This allows us to easily switch between renderers
  that make it possible to use the GUI in a OpenGL, DirectX, etc..
  application.

  See the comments for the functions below for their meaning. 

 */
#ifndef REMOXLY_RENDER_H
#define REMOXLY_RENDER_H

#include <sstream>
#include <string>

class Character;

class Render {

 public:
  virtual bool setup();       /* initialize the Render object */
  virtual void update();      /* update the vertices, GPU buffers, etc.. if somethings that gets drawn changed */
  virtual void draw();        /* draw everything to screen */

  /* masking / scrissor */
  virtual void getWindowSize(int& ww, int& wh);
  virtual void beginScissor();
  virtual void scissor(int sx, int sy, int sw, int sh);
  virtual void endScissor();

  /* interaction, e.g. text input */
  virtual void onCharPress(unsigned int key);
  virtual void onKeyPress(int key, int mods); 

  /* drawing and vertex management */
  virtual void clear();                                                                              /* removes all vertices, and offset data */
  virtual void getNumberInputValue(std::string& result);
  virtual void disableNumberInput();
  virtual void enableNumberInput(float x, float y, float maxw, std::string value, float* color);
  virtual void enableTextInput(float x, float y, float maxw, std::string value, float* color); 
  virtual void writeText(float x, float y, std::string text, float* color);                          /* write some text */
  virtual void writeNumber(float x, float y, std::string number, float* color);                      /* write a number; we will right align numbers */
  virtual void writeIcon(float x, float y, unsigned int icon, float* color);
  virtual bool getIconSize(unsigned int id, int& w, int& h);
  virtual void addRectangle(float x, float y, float w, float h, float* color, bool filled = true);   /* draw a rectangle at x/y with w/h and given color; must have 4 elements */

  /* helpers */
  template<class T> void writeNumber(float x, float y, T number, float* color);
  template<class T> void enableNumberInput(float x, float y, float maxw, T number, float* color);
  template<class T> void getNumberInputValue(T& value);
};

template<class T>
void Render::writeNumber(float x, float y, T number, float* color) {
  std::stringstream ss;
  std::string text;
  ss << number;
  ss >> text;
  writeNumber(x, y, text, color);
}

template<class T>
void Render::enableNumberInput(float x, float y, float maxw, T number, float* color) {
  std::stringstream ss;
  std::string text;
  ss << number;
  ss >> text;
  enableNumberInput(x, y, maxw, text, color);
}

template<class T>
void Render::getNumberInputValue(T& value) {
  std::string result;
  std::stringstream ss;
  getNumberInputValue(result);
  ss << result;
  ss >> value;
}


#endif
