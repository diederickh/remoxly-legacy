/*

  Remoxly
  --------

  A basic wrapper around the widgets, panels, groups, renderer and storage. 
  This class is created give uses a quick way of creating GUIs. Make sure
  to only create an instance of the Remoxly class -after- you have created
  an openGL context. 

  Also, you need to use "#define REMOXLY_IMPLEMENTATION" in at least one 
  CPP file after you've included the OpenGL headers. 

  <example>

    #define REMOXLY_IMPLEMENTATION
    #include <gui/Remoxly.h>

    Remoxly* gui = new Remoxly();
    gui->addGroup("Particles");
    gui->addToggle("Enable drawing", enable_drawing);

  </example>

 */

// ------------------------------------------------------------------------------
// H E A D E R
// ------------------------------------------------------------------------------

#ifndef REMOXLY_INCLUDES_H
#define REMOXLY_INCLUDES_H

#include <vector>
#include <string> 

#include <gui/Button.h>
#include <gui/ColorRGB.h>
#include <gui/Group.h>
#include <gui/IconButton.h>
#include <gui/WidgetListener.h>
#include <gui/Panel.h>
#include <gui/Render.h>
#include <gui/Scroll.h>
#include <gui/Slider.h>
#include <gui/Storage.h>
#include <gui/Text.h>
#include <gui/Texture.h>
#include <gui/Toggle.h>
#include <gui/Types.h>
#include <gui/Utils.h>
#include <gui/Widget.h>
#include <gui/storage/StorageXML.h>

#if defined(REMOXLY_IMPLEMENTATION)
#  include <gui/gl/ImplementationGL.h>
#endif

#define REMOXLY_CHECK_GROUP(err) { if (!curr_group) { printf("%s", err); return NULL; } } 

namespace rx {

  class Remoxly {

  public:
    Remoxly();
    ~Remoxly();

    void draw();
    bool save(std::string filename);
    bool load(std::string filename);
    void resize(int w, int h);

    /* widgets */
    Panel* addPanel(int h = 300);
    Group* addGroup(std::string label);
    Toggle* addToggle(std::string label, bool& value);
    Slider<int>* addSliderInt(std::string label, int& value, int minv, int maxv, int stepv);
    Slider<float>* addSliderFloat(std::string label, float& value, float minv, float maxv, float stepv);
    ColorRGB* addColor(std::string label, float* rgb, int ncolors = 50, float sat = 0.8, float val = 1.0);
    Button* addButton(std::string label, int id, unsigned int icon, gui_button_callback cb, void* user, int iconx = 0, int icony = 0);
    Text* addText(std::string label, std::string& value, int textWidth = 220);
    Texture* addTexture(std::string label, TextureInfo* texinfo);

    /* interaction */
    void onCharPress(unsigned int key);         
    void onKeyPress(int key, int modkeys);      
    void onKeyRelease(int key, int modkeys);
    void onMousePress(float mx, float my, int button, int modkeys);     
    void onMouseRelease(float mx, float my, int button, int modkeys);   
    void onMouseMove(float mx, float my);                               

  public:

    /* gui */
    Panel* curr_panel;
    Group* curr_group;
    std::vector<Panel*> panels;
    std::vector<Group*> groups;
  };

} // namespace rx  

#endif // REMOXLY_INCLUDES_H

// ------------------------------------------------------------------------------
// I M P L E M E N T A T I O N
// ------------------------------------------------------------------------------

#if defined(REMOXLY_IMPLEMENTATION)

namespace rx { 

  Remoxly::Remoxly() 
    :curr_panel(NULL)
    ,curr_group(NULL)
    {
    }

  Remoxly::~Remoxly() {
  }

  void Remoxly::draw() {

    for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
      (*it)->draw();
    }
  }

  bool Remoxly::save(std::string filename) {

    std::string filepath = filename;
    StorageXML xml(filepath);
  
    for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
      xml.addPanel(*it);
    }
  
    return xml.save();
  }

  bool Remoxly::load(std::string filename) {

    std::string filepath = filename;
    StorageXML xml(filepath);
  
    for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
      xml.addPanel(*it);
    }
  
    return xml.load();
  }

  Panel* Remoxly::addPanel(int h) {

    curr_panel = new Panel(new RenderGL(), h);
    curr_panel->x = panels.size() * (curr_panel->w + curr_panel->scroll.w)  + (1 + panels.size()) * 10;
    panels.push_back(curr_panel);
    return curr_panel;
  }

  Group* Remoxly::addGroup(std::string label) {

    if(!curr_panel) {
      addPanel();
    }

    if(!curr_panel) {
      printf("Error: no current panel set.\n");
      return NULL;
    }

    curr_group = curr_panel->addGroup(label);
    groups.push_back(curr_group);

    return curr_group;
  }

  Toggle* Remoxly::addToggle(std::string label, bool& value) {

    REMOXLY_CHECK_GROUP("Error: no group added, cannot add toggle.\n");

    Toggle* t = new Toggle(label, value);
    curr_group->add(t);
    return t;
  }

  Slider<int>* Remoxly::addSliderInt(std::string label, int& value, int minv, int maxv, int stepv) {

    REMOXLY_CHECK_GROUP("Error: no group added, cannot add slider<int>.\n");

    Slider<int>* s = new Slider<int>(label, value, minv, maxv, stepv);
    curr_group->add(s);
    return s;
  }

  Slider<float>* Remoxly::addSliderFloat(std::string label, float& value, float minv, float maxv, float stepv) {

    REMOXLY_CHECK_GROUP("Error: no group added, cannot add slider<float>.\n");

    Slider<float>* s = new Slider<float>(label, value, minv, maxv, stepv);
    curr_group->add(s);
    return s;
  }

  ColorRGB* Remoxly::addColor(std::string label, float* rgb, int ncolors, float sat, float val) {

    REMOXLY_CHECK_GROUP("Error: no group added, cannot add a color.\n");

    ColorRGB* c = new ColorRGB(label, rgb, ncolors, sat, val);
    curr_group->add(c);
    return c;
  }

  Button* Remoxly::addButton(std::string label, int id, unsigned int icon, gui_button_callback cb, void* user, int iconx, int icony) {

    REMOXLY_CHECK_GROUP("Error: no group added, cannot add a button.\n");
  
    Button* b = new Button(label, id, icon, cb, user, iconx, icony);
    curr_group->add(b);
    return b;
  }

  Text* Remoxly::addText(std::string label, std::string& value, int textWidth) {
  
    REMOXLY_CHECK_GROUP("Error: no group added, cannot add a text input.\n");
  
    Text* t = new Text(label, value, textWidth);
    curr_group->add(t);
    return t;
  }

  Texture* Remoxly::addTexture(std::string label, TextureInfo* texinfo) { 

    REMOXLY_CHECK_GROUP("Error: no group added, cannot add a texture.\n");

    Texture* t = new Texture(label, texinfo);
    curr_group->add(t);

    return t;
  }

  void Remoxly::onCharPress(unsigned int key) { 

    for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
      (*it)->onCharPress(key);
    }
  }

  void Remoxly::onKeyRelease(int key, int modkeys) { 

    for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
      (*it)->onKeyRelease(key, modkeys);
    }
  }

  void Remoxly::onKeyPress(int key, int modkeys) { 

    for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
      (*it)->onKeyPress(key, modkeys);
    }
  }

  void Remoxly::onMouseMove(float mx, float my) {

    for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
      (*it)->onMouseMove(mx, my);
    }
  }

  void Remoxly::onMousePress(float mx, float my, int button, int modkeys) { 

    for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
      (*it)->onMousePress(mx, my, button, modkeys);
    }
  }

  void Remoxly::onMouseRelease(float mx, float my, int button, int modkeys) { 

    for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
      (*it)->onMouseRelease(mx, my, button, modkeys);
    }
  }

  void Remoxly::resize(int w, int h) {
    for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
      (*it)->render->resize(w, h);
    }
  }

} // namespace rx

#endif // IMPLEMENTATION
