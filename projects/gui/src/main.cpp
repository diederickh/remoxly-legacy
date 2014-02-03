/*

  Gui
  ---

  This class is used for the development of the Gui and its
  widgets. It's not supposed to be an simple example because we 
  needs to add some code to setup an opengl context, include some
  implementation etc.. Though if you want to create new widgets
  this is a good place to start. Make sure to add the cpp files 
  to the build/CMakeLists.txt example and read the documenation
  on how to write widgets.

 */
#include <stdlib.h>
#include <stdio.h>

#if defined(__linux) || defined(_WIN32)
#  include <GLXW/glxw.h>
#endif
 
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include <gui/Remoxly.h>
#include <gui/gl/ImplementationGL.h>

#define USE_GUI 0
#define USE_PANEL 1
Gui* gui_ptr0 = NULL;
Gui* gui_ptr1 = NULL;
Panel* panel_ptr = NULL;

#define ROXLU_USE_ALL
#define ROXLU_IMPLEMENTATION
#include <tinylib.h>
 
void button_callback(GLFWwindow* win, int bt, int action, int mods);
void cursor_callback(GLFWwindow* win, double x, double y);
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods);
void char_callback(GLFWwindow* win, unsigned int key);
void error_callback(int err, const char* desc);
void resize_callback(GLFWwindow* window, int width, int height);

void on_gui_button_click(int id, void* user);
 
int main() {

  glfwSetErrorCallback(error_callback);
 
  if(!glfwInit()) {
    printf("Error: cannot setup glfw.\n");
    return false;
  }
 
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  GLFWwindow* win = NULL;
  int w = 1280;
  int h = 720;
 
  win = glfwCreateWindow(w, h, "Gui", NULL, NULL);
  if(!win) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetFramebufferSizeCallback(win, resize_callback);
  glfwSetKeyCallback(win, key_callback);
  glfwSetCharCallback(win, char_callback);
  glfwSetCursorPosCallback(win, cursor_callback);
  glfwSetMouseButtonCallback(win, button_callback);
  glfwMakeContextCurrent(win);
  glfwSwapInterval(1);
 
#if defined(__linux) || defined(_WIN32)
  if(glxwInit() != 0) {
    printf("Error: cannot initialize glxw.\n");
    ::exit(EXIT_FAILURE);
  }
#endif

  // ----------------------------------------------------------------
  // THIS IS WHERE YOU START CALLING OPENGL FUNCTIONS, NOT EARLIER!!
  // ----------------------------------------------------------------
  int forces = 36;
  int velocity = 4;
  int amount = 9;
  int lifetime = 50;
  float mass = 79.5f;
  float aging = 44.50f;
  int trail_size = 350;
  float start_size = 1.0f;
  float end_size = 10.0f;
  float start_angle = 0.15f;
  float end_angle = 3.1415f;
  float line_size = 100.0f;
  bool render_particles = true;
  bool render_water = false;
  bool render_spirals = false;
  bool render_marker = true;
  float color[3] = { 0 } ;

#if USE_GUI
  Gui gui0("Particles", new RenderGL());
  gui0.add(new Slider<int>("Particle Forces", forces, 0, 100, 1));
  gui0.add(new Slider<int>("Particle Velocity", velocity, 0, 100, 1));
  gui0.add(new Slider<int>("Particle Amount", amount, 0, 10, 1));
  gui0.add(new Slider<int>("Particle Lifetime", lifetime, 0, 10, 1));
  gui0.add(new Slider<float>("Particle Mass", mass, 0.0f, 100.0f, 0.001f));

  if(!gui0.setup()) {
    printf("Error: cannot setup gui.\n");
    ::exit(EXIT_FAILURE);
  }

  gui_ptr0 = &gui0;

  Gui gui1("Drawings", new RenderGL());
  gui1.add(new Slider<float>("Line size", line_size, 0.0f, 200.0f, 0.1f));
  gui1.x = 300;
  gui_ptr1 = &gui1;
  gui1.setup();

#endif

#if USE_PANEL
  Panel panel(new RenderGL(), 300);

  Gui* g0 = panel.addGui("Particles");
  g0->add(new ColorRGB("Color (work in progress)", color));
  g0->add(new Button("Save Settings", 3, GUI_ICON_FLOPPY_O, on_gui_button_click, NULL, 1));
  g0->add(new Button("Load Settings", 4, GUI_ICON_REFRESH, on_gui_button_click, NULL, 1));
  g0->add(new Slider<int>("Particle velocity", velocity, 0, 100, 1));

  Gui* g1 = panel.addGui("Water Simulation");
  g1->add(new Slider<int>("Particle Lifetime", lifetime, 0, 10, 1));
  g1->add(new Slider<int>("Particle Amount", amount, 0, 10, 1));
  g1->add(new Toggle("Render Particles", render_particles));
  g1->add(new Toggle("Render Water", render_water));
  g1->add(new Toggle("Render Spirals", render_spirals));
  g1->add(new Toggle("Render Marker", render_marker));

  Gui* g2 = panel.addGui("Special FX");

  Slider<float>* aging_slider = new Slider<float>("Aging", aging, 0.0f, 50.0f, 0.1f);
  //aging_slider->hide();

  g2->add(aging_slider);
  g2->add(new Slider<int>("Trail", trail_size, 0, 450, 1));
  
  int num = 1;
  for(int i = 0; i < num; ++i) {
    g2->add(new Slider<float>("Start Size", start_size, 0.0f, 50.0f, 0.1f));
    g2->add(new Slider<float>("End Size", end_size, 0.0f, 50.0f, 0.1f));
    g2->add(new Slider<float>("Begin Angle", start_angle, 0.0f, 150.0f, 0.1f));
    g2->add(new Slider<float>("End Angle", end_angle, 0.0f, 200.0f, 0.1f));
  }

  g2->add(new Button("Download", 0, GUI_ICON_DOWNLOAD, on_gui_button_click, NULL, 1));
  g2->add(new Button("Twitter", 1, GUI_ICON_TWITTER, on_gui_button_click, NULL, 1));
  g2->add(new Button("Cloud Burst", 2, GUI_ICON_CLOUD, on_gui_button_click, NULL, 1));

  panel_ptr = &panel;
#endif

  while(!glfwWindowShouldClose(win)) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if USE_GUI
    gui0.draw();
    gui1.draw();
#endif

#if USE_PANEL
    panel.draw();
#endif

    glfwSwapBuffers(win);
    glfwPollEvents();
  }
 
  glfwTerminate();
 
  return EXIT_SUCCESS;
}
 
void char_callback(GLFWwindow* win, unsigned int key) {

  if(gui_ptr0) {
    gui_ptr0->onCharPress(key);
  }

  if(gui_ptr1) {
    gui_ptr1->onCharPress(key);
  }

  if(panel_ptr) {
    panel_ptr->onCharPress(key);
  }
}
 
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
  
  if(action == GLFW_RELEASE) {
    if(gui_ptr0) {
      gui_ptr0->onKeyRelease(key, mods);
    }
    if(gui_ptr1) {
      gui_ptr1->onKeyRelease(key, mods);
    }
    if(panel_ptr) {
      panel_ptr->onKeyRelease(key, mods);
    }
    return;
  }

  if( (action != GLFW_PRESS) && (action != GLFW_REPEAT) ) {
    return;
  }

  if(gui_ptr0) {
    gui_ptr0->onKeyPress(key, mods);
  }

  if(gui_ptr1) {
    gui_ptr1->onKeyPress(key, mods);
  }

  if(panel_ptr) {
    panel_ptr->onKeyPress(key, mods);
    if(key == GLFW_KEY_F1) {
      panel_ptr->needs_redraw = true;
    }
  }

  switch(key) {
    case GLFW_KEY_ESCAPE: {
      glfwSetWindowShouldClose(win, GL_TRUE);
      break;
    }
  };
}
 
void resize_callback(GLFWwindow* window, int width, int height) {
}

void cursor_callback(GLFWwindow* win, double x, double y) {

  if(gui_ptr0) {
    gui_ptr0->onMouseMove(x, y);
  }

  if(gui_ptr1) {
    gui_ptr1->onMouseMove(x, y);
  }


  if(panel_ptr) {
    panel_ptr->onMouseMove(x, y);
  }
}

void button_callback(GLFWwindow* win, int bt, int action, int mods) {

  double mx = 0;
  double my = 0;

  glfwGetCursorPos(win, &mx, &my);
  
  if(action == GLFW_PRESS) {
    if(gui_ptr0) {
      gui_ptr0->onMousePress(mx, my, bt, mods);
    }
    if(gui_ptr1) {
      gui_ptr1->onMousePress(mx, my, bt, mods);
    }

    if(panel_ptr) {
      panel_ptr->onMousePress(mx, my, bt, mods);
    }
  }
  else if(action == GLFW_RELEASE) {
    if(gui_ptr0) {
      gui_ptr0->onMouseRelease(mx, my, bt, mods);
    }
    if(gui_ptr1) {
      gui_ptr1->onMouseRelease(mx, my, bt, mods);
    }
    if(panel_ptr) {
      panel_ptr->onMouseRelease(mx, my, bt, mods);
    }
  }
}

void error_callback(int err, const char* desc) {
  printf("GLFW error: %s (%d)\n", desc, err);
}

void on_gui_button_click(int id, void* user) {

  if(id == 3 && panel_ptr) {
    StorageXML xml("panel.xml");
    xml.addPanel(panel_ptr);
    xml.save();
    printf("Saved.\n");
  }
  else if(id == 4 && panel_ptr) {
    StorageXML xml("panel.xml");
    xml.addPanel(panel_ptr);
    xml.load();
    printf("Loaded.\n");
  }
}
