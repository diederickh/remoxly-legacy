/*

  ofxRemoxly
  -----------

  ofxRemoxly is a wrapper around the [Remoxly GUI](http://remoxly.roxlu.com). 
  Remoxly is a clean, utilitarian GUI with basic gui widgets like toggles, 
  buttons, sliders (int and float), text input and a basic color picker. 

  Remoxly allows you to control your GUI through a remote connection. 

  <example>

    gui.addGroup("Particles");
    gui.addColor("Color", color);
    gui.addSliderFloat("Speed", particle_speed, 0.0, 10.0, 0.1);
    gui.addSliderFloat("Trail width", trail_width, 0.0, 100.0, 0.01);
    gui.addSliderInt("Trail length", trail_size, 0, 500, 1);
    gui.addSliderInt("Circle Radius", circle_radius, 0, 300, 1);
    gui.addToggle("Enable particles", enable_particles);
    gui.addToggle("Render particles", render_particles);
    gui.addToggle("Render trails", render_trails);
    gui.addToggle("Render circle", render_circle);
  
    gui.addGroup("Save and Load");
    gui.addButton("Love", 0, GUI_ICON_HEART, on_button_click, this, 1);
    gui.addButton("Save", 1, GUI_ICON_FLOPPY_O, on_button_click, this, 1);
    gui.addButton("Load", 2, GUI_ICON_REFRESH, on_button_click, this,2);
  
    gui.addPanel();
    gui.addGroup("Mesh");
    gui.addSliderInt("Number of triangles", number_of_triangles, 0, 300, 3);
    gui.addSliderFloat("Minimum Triangle Space", min_triangle_space, 0.0f, 100.0f, 1.0f);
    gui.addToggle("Triangulate", triangulate);
    gui.addText("Profile name", profile_name, 180);
  
    gui.enableRemoteAccess(2244);
  
    gui.load("gui.xml");

  </example>
  
 */
#ifndef REMOXLY_OF_REMOXLY_H
#define REMOXLY_OF_REMOXLY_H

#include "ofMain.h"

#include <gui/Remoxly.h>
#include <gui/remote/Remote.h>

#include <string>
#include <vector>

#define OFX_REMOXLY_CHECK_GROUP(err) { if (!curr_group) { printf("%s", err); return NULL; } } 

class ofxRemoxly {

 public:
  ofxRemoxly();
  ~ofxRemoxly();

  void draw();
  bool save(std::string filename);
  bool load(std::string filename);
  void resize(int w, int h);

  bool enableRemoteAccess(int port);

  /* widgets */
  Panel* addPanel(int h = 300);
  Group* addGroup(std::string label);
  Toggle* addToggle(std::string label, bool& value);
  Slider<int>* addSliderInt(std::string label, int& value, int minv, int maxv, int stepv);
  Slider<float>* addSliderFloat(std::string label, float& value, float minv, float maxv, float stepv);
  ColorRGB* addColor(std::string label, ofFloatColor& color, int ncolors = 50, float sat = 0.8, float val = 1.0);
  Button* addButton(std::string label, int id, unsigned int icon, gui_button_callback cb, void* user, int iconx = 0, int icony = 0);
  Text* addText(std::string label, std::string& value, int textWidth = 220);
  Texture* addTexture(std::string label, ofImage& img);

  /* interaction */
  void keyReleased(ofKeyEventArgs& args);
  void keyPressed(ofKeyEventArgs& args);
  void mouseDragged(ofMouseEventArgs& args);
  void mouseMoved(ofMouseEventArgs& args);
  void mousePressed(ofMouseEventArgs& args);
  void mouseReleased(ofMouseEventArgs& args);
  

  /* mapping of keys */
  int ofKeyToRemoxlyKey(int ofkey);

 public:

  /* gui */
  Panel* curr_panel;
  Group* curr_group;
  std::vector<Panel*> panels;
  std::vector<Group*> groups;

  /* client <> server */
  Server* server;
  Client* client;
};

#endif
