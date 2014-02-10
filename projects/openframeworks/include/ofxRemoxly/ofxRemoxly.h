/*

  ofxRemoxly
  -----------


  

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
  ColorRGB* addColor(std::string label, ofFloatColor& color);
  Button* addButton(std::string label, int id, unsigned int icon, gui_button_callback cb, void* user, int iconx = 0, int icony = 0);
  Text* addText(std::string label, std::string& value, int textWidth = 220);

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
