#ifndef REMOXLY_REMOTE_CLIENT_APP_H
#define REMOXLY_REMOTE_CLIENT_APP_H

extern "C" { 
#  include <jansson.h>
}

#include <string>
#include <vector>
#include <map>
#include <stdint.h>
#include <gui/gl/ImplementationGL.h>
#include <gui/remote/Remote.h>

using namespace rx;

struct ColorRGBValue {
  ColorRGBValue():r(0.0f),g(0.0f),b(0.0f){}
  float r;
  float g;
  float b;
};

void remote_client_app_button_callback(int id, void* user);
void remote_client_app_connect_callback(int id, void* user);

class RemoteClientApp : public ClientListener, public Generator {

 public:
  RemoteClientApp(std::string host, int port);

  void connect();
  void update();
  void draw();

  /* interaction */
  void onCharPress(unsigned int key);
  void onKeyPress(int key, int modkeys);
  void onKeyRelease(int key, int modkeys);
  void onMousePress(float mx, float my, int button, int modkeys);
  void onMouseRelease(float mx, float my, int button, int modkeys);
  void onMouseMove(float mx, float my);

  /* client listener */
  void onTaskSetGuiModel(char* data, size_t len, std::string value);  
  void onDisconnected();
  template<class T> void deleteHeap(std::vector<T*>& els);

  /* generator */
  Panel*          createPanel(int h = 200);
  Group*          createGroup(Panel* panel, std::string label);
  Group*          createGroup(std::string label);
  Slider<int>*    createSliderInt(std::string label, int minv, int maxv, int step, int id);
  Slider<float>*  createSliderFloat(std::string label, float minv, float maxv, float step, int id);
  Toggle*         createToggle(std::string label, int id);
  ColorRGB*       createColorRGB(std::string label, int id, int ncolors, float sat, float val);
  Button*         createButton(std::string label, int id, int buttonID, unsigned int icon);
  Text*           createText(std::string label, int id, int textw);

 public:
  Client client;
  Deserializer deserializer;
  std::vector<Panel*> panels;
  std::vector<Group*> groups;
  std::vector<float*> float_values;
  std::vector<int*> int_values;
  std::vector<bool*> bool_values;
  std::vector<ColorRGBValue*> color_values;
  std::vector<std::string*> string_values;

  Group gui;
  std::string server_host;
  std::string server_port;
};

// --------------------------------------------------------
void remote_client_app_button_callback(int id, void* user) {
}

void remote_client_app_connect_callback(int id, void* user) {

  RemoteClientApp* app = static_cast<RemoteClientApp*>(user);

  StorageXML xml("remote_control.xml");
  xml.addGroup(&app->gui);
  xml.save();
 
  app->connect();
}

// --------------------------------------------------------

RemoteClientApp::RemoteClientApp(std::string host, int ip) 
  :client(host, ip, false, this)
  ,deserializer(this)
  ,gui("Connect", new RenderGL())
{ 
  gui.add(new Text("Host", server_host));
  gui.add(new Text("Port", server_port));
  gui.add(new Button("Connect", 0, GUI_ICON_BOLT, remote_client_app_connect_callback, this));
  gui.lockPosition();

  client.auto_reconnect = false;

  StorageXML xml("remote_control.xml");
  xml.addGroup(&gui);
  xml.load();
}

void RemoteClientApp::connect() {
  client.host = server_host;
  client.port = gui_string_to_int(server_port);
  client.connect();
}

void RemoteClientApp::update() {
  client.update();
}

void RemoteClientApp::draw() {

  for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
    (*it)->draw();
  }
  
  if(!client.isConnected()) {
    int win_w = 0;
    int win_h = 0;
    gui.render->getWindowSize(win_w, win_h);
    gui.x = (win_w * 0.5) - (gui.w * 0.5);
    gui.y = 200;
    gui.draw();
  }
}

void RemoteClientApp::onTaskSetGuiModel(char* data, size_t len, std::string value) {

  // this will create all widgets.
  if(!deserializer.deserialize(value)) {
    printf("Error: cannot deserialize the gui model.\n");
    return ;
  }

  // once all widgets have been created we add the panels/group to the client so all values are send over the network
  for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
    client.addPanel((*it));
  }
}

// --------------------------------------------------------

Panel* RemoteClientApp::createPanel(int h) {
  Panel* p = new Panel(new RenderGL(), h);
  p->x = panels.size() * (p->w + p->scroll.w) + (panels.size() + 1) * 10;
  panels.push_back(p);
  return p;
}

Group* RemoteClientApp::createGroup(Panel* panel, std::string label) {
  return panel->addGroup(label);
}

Group* RemoteClientApp::createGroup(std::string label) {
  return NULL;
}

Slider<int>* RemoteClientApp::createSliderInt(std::string label, int minv, int maxv, int step, int id) {

  int* v = new int;
  int_values.push_back(v);
  *v = 0;

  Slider<int>* s = new Slider<int>(label, *v, minv, maxv, step);

  return s;
}

Slider<float>* RemoteClientApp::createSliderFloat(std::string label, float minv, float maxv, float step, int id) {  

  float* v = new float;
  float_values.push_back(v);    
  *v = 0.0f;
  
  Slider<float>* s = new Slider<float>(label, *v, minv, maxv, step);

  return s;
}

Toggle* RemoteClientApp::createToggle(std::string label, int id) {

  bool* v = new bool;
  bool_values.push_back(v);
  *v = false;

  Toggle* t = new Toggle(label, *v);

  return t;
}

ColorRGB* RemoteClientApp::createColorRGB(std::string label, int id, int ncolors, float sat, float val) {

  ColorRGBValue* v = new ColorRGBValue();
  color_values.push_back(v);

  ColorRGB* col = new ColorRGB(label, &v->r, ncolors, sat, val);
  return col;
}

Button* RemoteClientApp::createButton(std::string label, int id, int buttonID, unsigned int icon) {

  Button* button = new Button(label, buttonID, icon, remote_client_app_button_callback, this);
  return button;
}

Text* RemoteClientApp::createText(std::string label, int id, int textw) {
  
  std::string* v = new std::string;
  string_values.push_back(v);

  Text* t = new Text(label, *v, textw);
  return t;
}

void RemoteClientApp::onCharPress(unsigned int key) {
  
  if(!client.isConnected()) {
    gui.onCharPress(key);
    return;
  }

  for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
    (*it)->onCharPress(key);
  }
}

void RemoteClientApp::onKeyPress(int key, int modkeys) {

  if(!client.isConnected()) {
    gui.onKeyPress(key, modkeys);
    return;
  }

  for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
    (*it)->onKeyPress(key, modkeys);
  }
}

void RemoteClientApp::onKeyRelease(int key, int modkeys) {

  if(!client.isConnected()) {
    gui.onKeyRelease(key, modkeys);
    return;
  }

  for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
    (*it)->onKeyPress(key, modkeys);
  }
}

void RemoteClientApp::onMousePress(float mx, float my, int button, int modkeys) {

  if(!client.isConnected()) {
    gui.onMousePress(mx, my, button ,modkeys);
    return;
  }

  for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
    (*it)->onMousePress(mx, my, button, modkeys);
  }
}

void RemoteClientApp::onMouseRelease(float mx, float my, int button, int modkeys) {

  if(!client.isConnected()) {
    gui.onMouseRelease(mx, my, button ,modkeys);
    return;
  }

  for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
    (*it)->onMouseRelease(mx, my, button, modkeys);
  }
}

void RemoteClientApp::onMouseMove(float mx, float my) {
  for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
    (*it)->onMouseMove(mx, my);
  }
}

void RemoteClientApp::onDisconnected() {

  deleteHeap<Panel>(panels);
  deleteHeap<Group>(groups);
  deleteHeap<float>(float_values);
  deleteHeap<int>(int_values);
  deleteHeap<bool>(bool_values);
  deleteHeap<ColorRGBValue>(color_values);
  deleteHeap<std::string>(string_values);
}

template<class T> 
void RemoteClientApp::deleteHeap(std::vector<T*>& els) {
  if(els.size()) {
    for(typename std::vector<T*>::iterator it = els.begin(); it != els.end(); ++it) {
      T* el = *it;
      delete el;
      el = NULL;
    }
    els.clear();
  }
}

#endif
