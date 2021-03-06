/*

  Panel
  ------

  A panel is a container for multiple Groups. The panel will add a 
  scrollbar so you can add a lot of Groups. For now make sure that 
  you use a new render object for each panel you create!

  IMPORTANT: I repeat: for each panel you create, you need to 
  add a new Render object. Each Render object will keep it's 
  own state (vbo/vao) for the panel (or group) that is drawn.
  For performance issues the renderer should make sure that 
  it only initialize the shader/programs once. 

 */

#ifndef REMOXLY_GUI_PANEL_H
#define REMOXLY_GUI_PANEL_H

#include <vector>
#include <gui/Widget.h>
#include <gui/Group.h>
#include <gui/Scroll.h>

namespace rx { 

class Panel : public Widget {

 public:
  Panel(Render* render, int h = 200, int style = GUI_CORNER_ALL);
  ~Panel();

  Group* addGroup(std::string title, int style = GUI_CORNER_ALL);

  void create();
  void draw();
  void position();

  void onMousePress(float mx, float my, int button, int modkeys);
  void onMouseRelease(float mx, float my, int button, int modkeys);
  void onMouseMove(float mx, float my);
  bool needsRedraw();

 private:
  void update();

 public:
  Scroll scroll;
  std::vector<Group*> groups;
};

} // namespace rx

#endif
