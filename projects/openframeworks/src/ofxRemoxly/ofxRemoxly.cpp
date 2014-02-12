#include <ofxRemoxly/ofxRemoxly.h>
#include <iostream>

#define RENDER_GL RENDER_GL2
#include <gui/gl/ImplementationGL.h>

namespace rx { 

ofxRemoxly::ofxRemoxly() 
  :curr_panel(NULL)
  ,curr_group(NULL)
  ,server(NULL)
  ,client(NULL)
{
  ofRegisterMouseEvents(this);
  ofRegisterKeyEvents(this);
}

ofxRemoxly::~ofxRemoxly() {
}

void ofxRemoxly::draw() {

  if(server) {
    server->update();
  }

  if(client) {
    client->update();
  }

  for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
    (*it)->draw();
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glUseProgram(0);
}

bool ofxRemoxly::save(std::string filename) {

  std::string filepath = ofToDataPath(filename);
  StorageXML xml(filepath);
  
  for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
    xml.addPanel(*it);
  }
  
  return xml.save();
}

bool ofxRemoxly::load(std::string filename) {

  std::string filepath = ofToDataPath(filename);
  StorageXML xml(filepath);
  
  for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
    xml.addPanel(*it);
  }
  
  return xml.load();
}

Panel* ofxRemoxly::addPanel(int h) {

  curr_panel = new Panel(new RenderGL(), h);
  curr_panel->x = panels.size() * (curr_panel->w + curr_panel->scroll.w)  + (1 + panels.size()) * 10;
  panels.push_back(curr_panel);
  return curr_panel;
}

Group* ofxRemoxly::addGroup(std::string label) {

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

Toggle* ofxRemoxly::addToggle(std::string label, bool& value) {

  OFX_REMOXLY_CHECK_GROUP("Error: no group added, cannot add toggle.\n");

  Toggle* t = new Toggle(label, value);
  curr_group->add(t);
  return t;
}

Slider<int>* ofxRemoxly::addSliderInt(std::string label, int& value, int minv, int maxv, int stepv) {

  OFX_REMOXLY_CHECK_GROUP("Error: no group added, cannot add slider<int>.\n");

  Slider<int>* s = new Slider<int>(label, value, minv, maxv, stepv);
  curr_group->add(s);
  return s;
}

Slider<float>* ofxRemoxly::addSliderFloat(std::string label, float& value, float minv, float maxv, float stepv) {

  OFX_REMOXLY_CHECK_GROUP("Error: no group added, cannot add slider<float>.\n");

  Slider<float>* s = new Slider<float>(label, value, minv, maxv, stepv);
  curr_group->add(s);
  return s;
}

ColorRGB* ofxRemoxly::addColor(std::string label, ofFloatColor& color, int ncolors, float sat, float val) {

  OFX_REMOXLY_CHECK_GROUP("Error: no group added, cannot add a color.\n");

  ColorRGB* c = new ColorRGB(label, (float*)&color.r, ncolors, sat, val);
  curr_group->add(c);
  return c;
}

Button* ofxRemoxly::addButton(std::string label, int id, unsigned int icon, gui_button_callback cb, void* user, int iconx, int icony) {

  OFX_REMOXLY_CHECK_GROUP("Error: no group added, cannot add a button.\n");
  
  Button* b = new Button(label, id, icon, cb, user, iconx, icony);
  curr_group->add(b);
  return b;
}

Text* ofxRemoxly::addText(std::string label, std::string& value, int textWidth) {
  
  OFX_REMOXLY_CHECK_GROUP("Error: no group added, cannot add a text input.\n");
  
  Text* t = new Text(label, value, textWidth);
  curr_group->add(t);
  return t;
}

Texture* ofxRemoxly::addTexture(std::string label, ofImage& img) {

  OFX_REMOXLY_CHECK_GROUP("Error: no group added, cannot add a texture.\n");

  ofTexture& tex = img.getTextureReference();
  ofTextureData& texdata = tex.getTextureData();

  Texture* t = new Texture(label, new TextureInfoGL(texdata.textureTarget, texdata.textureID));
  curr_group->add(t);

  printf("target: %d = %d, id: %d\n", GL_TEXTURE_2D, texdata.textureTarget, texdata.textureID);
  return t;
}

void ofxRemoxly::keyReleased(ofKeyEventArgs& args) {

  for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
    int key = ofKeyToRemoxlyKey(args.key);
    if(key) {
      (*it)->onKeyRelease(key, 0);
    }
  }
}

void ofxRemoxly::keyPressed(ofKeyEventArgs& args) {
  for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
    int key = ofKeyToRemoxlyKey(args.key);
    if(!key) {
      (*it)->onCharPress(args.key);
    }
    else {
      (*it)->onKeyPress(key, 0);
    }
  }
}

void ofxRemoxly::mouseDragged(ofMouseEventArgs& args) {
  for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
    (*it)->onMouseMove(args.x, args.y);
  }
}

void ofxRemoxly::mouseMoved(ofMouseEventArgs& args) {
  for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
    (*it)->onMouseMove(args.x, args.y);
  }
}

void ofxRemoxly::mousePressed(ofMouseEventArgs& args) {
  for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
    (*it)->onMousePress(args.x, args.y, args.button, 0);
  }
}

void ofxRemoxly::mouseReleased(ofMouseEventArgs& args) {
  for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
    (*it)->onMouseRelease(args.x, args.y, args.button, 0);
  }
}

bool ofxRemoxly::enableRemoteAccess(int port) {

  if(server) {
    printf("Error: already enabled remote access for the gui.\n");
    return false;
  }

  if(!panels.size()) {
    printf("Error: trying to enable remote access before you've added any panels/groups/widgets.\n");
    return false;
  }

  server = new Server(port, false);

  if(!server->start()) {
    printf("Error: cannot create the server that is needed for remote access.\n");
    delete server;
    server = NULL;
    return false;

  }

  client = new Client("127.0.0.1", port, false);

  for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
    client->addPanel(*it);
  }

  if(!client->connect()) {
    printf("Error: cannot create the client that is needed for remote access.\n");
    delete client;
    delete server;
    client = NULL;
    server = NULL;
    return false;
  }

  return true;
}

int ofxRemoxly::ofKeyToRemoxlyKey(int ofkey) {
  switch(ofkey) {
    case OF_KEY_LEFT:       return GUI_KEY_LEFT;
    case OF_KEY_RIGHT:      return GUI_KEY_RIGHT;
    case OF_KEY_DEL:        return GUI_KEY_DELETE;
    case OF_KEY_BACKSPACE:  return GUI_KEY_BACKSPACE;
    case OF_KEY_RETURN:     return GUI_KEY_ENTER;
    default:                return 0;
  }
}

void ofxRemoxly::resize(int w, int h) {
  for(std::vector<Panel*>::iterator it = panels.begin(); it != panels.end(); ++it) {
    (*it)->render->resize(w, h);
  }
}

} // namespace rx 
