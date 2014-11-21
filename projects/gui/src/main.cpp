/*

  Group
  ---

  This class is used for the development of the Group and its
  widgets. It's not supposed to be an simple example because we 
  needs to add some code to setup an opengl context, include some
  implementation etc.. Though if you want to create new widgets
  this is a good place to start. Make sure to add the cpp files 
  to the build/CMakeLists.txt example and read the documenation
  on how to write widgets.

 */
#include <stdlib.h>
#include <stdio.h>

#include "App.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define REMOXLY_USE_OPENGL
#define REMOXLY_IMPLEMENTATION
#include <gui/Remoxly.h>

using namespace rx;

#define USE_GROUP 1
#define USE_PANEL 1
#define USE_CONTAINER 1

Group* group_ptr0 = NULL;
Group* group_ptr1 = NULL;
Panel* panel_ptr = NULL;
Container* container_ptr = NULL;
Remoxly* remoxly_ptr = NULL;

#define ROXLU_USE_OPENGL
#define ROXLU_IMPLEMENTATION
#include <tinylib.h>
 
void button_callback(GLFWwindow* win, int bt, int action, int mods);
void cursor_callback(GLFWwindow* win, double x, double y);
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods);
void char_callback(GLFWwindow* win, unsigned int key);
void error_callback(int err, const char* desc);
void resize_callback(GLFWwindow* window, int width, int height);

void on_group_button_click(int id, void* user);
void on_select_click(int selectid, int optionid, void* user);
 
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
 
  win = glfwCreateWindow(w, h, "Group", NULL, NULL);
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

  if (!gladLoadGL()) {
    printf("Cannot load GL.\n");
    exit(1);
  }

  // ----------------------------------------------------------------
  // THIS IS WHERE YOU START CALLING OPENGL FUNCTIONS, NOT EARLIER!!
  // ----------------------------------------------------------------
  glEnable(GL_CULL_FACE);

  float forces = 1.0;
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
  std::string rtmp_host;

  std::vector<std::string> options;
  options.push_back("Logitech C920");
  options.push_back("Decklink Capture");
  options.push_back("CamCorder");

#if USE_GROUP
  Group group0("Particles", new RenderGL());
  group0.add(new Slider<float>("Particle Forces", forces, 0.01f, 10.0f, 0.01f));
  group0.y = 330;
  group_ptr0 = &group0;

  Select* sel = new Select("Webcam 1", 1, options, on_select_click, NULL, GUI_CORNER_ALL);
  sel->setDirection(GUI_DIRECTION_UP);
  group0.add(sel);

  group0.add(new Slider<int>("Particle Velocity", velocity, 0, 100, 1));
  group_ptr0 = &group0;  
  
  group0.add(new Slider<int>("Particle Amount", amount, 4, 10, 1));
  group0.add(new Slider<int>("Particle Lifetime", lifetime, 0, 10, 1));
  group0.add(new Slider<float>("Particle Mass", mass, 0.0f, 100.0f, 0.001f));

  Group group1("Drawings", new RenderGL());
  group1.add(new Slider<float>("Line size", line_size, 0.0f, 200.0f, 0.1f));
  group1.x = 10;
  group1.y = 600;
  group_ptr1 = &group1;

#endif

#if USE_PANEL
  Panel panel(new RenderGL(), 300);

  Group* g0 = panel.addGroup("Particles");
  g0->add(new ColorRGB("Pastel Colors", color, 50, 0.5f));
  g0->add(new ColorRGB("Bright Colors", color, 150, 0.9f, 1.0f));
  g0->add(new ColorRGB("Limited Range", color, 15, 0.7f, 0.9f)).setMarginBottom(10);
  g0->add(new Button("Save Settings", 3, GUI_ICON_FLOPPY_O, on_group_button_click, NULL, GUI_CORNER_TOP));
  g0->add(new Button("Load Settings", 4, GUI_ICON_REFRESH, on_group_button_click, NULL, GUI_CORNER_BOTTOM)).setMarginBottom(10);
  g0->add(new Slider<int>("Particle velocity", velocity, 0, 100, 1));
  g0->add(new Text("RTMP host", rtmp_host));

  Group* g1 = panel.addGroup("Water Simulation");
  //g1->hideHeader();

  Slider<int>* test_slider = new Slider<int>("Particle Lifetime", lifetime, 0, 10, 1);
  g1->add(test_slider);
#if 1
  g1->add(new Slider<int>("Particle Amount", amount, 0, 10, 1)).setMarginBottom(10);
  g1->add(new Select("Webcam A", 2, options, on_select_click, NULL, GUI_STYLE_NONE));
  g1->add(new Select("Webcam B", 3, options, on_select_click, NULL, GUI_STYLE_NONE));
  g1->add(new Toggle("Render Particles", render_particles));
  g1->add(new Toggle("Render Water", render_water));
  g1->add(new Toggle("Render Spirals", render_spirals));
  g1->add(new Toggle("Render Marker", render_marker));
#endif

  Group* g2 = panel.addGroup("Special FX");
  g0->padding = 1;
  g1->padding = 1;
  g2->padding = 1;

  g0->setMarginBottom(10);
  g1->setMarginBottom(10);
  g2->setMarginBottom(10);

#if 1
  Slider<float>* aging_slider = new Slider<float>("Aging", aging, 0.0f, 50.0f, 0.1f);
  //aging_slider->hide();

  g2->add(aging_slider);
  g2->add(new Slider<int>("Trail", trail_size, 0, 450, 1));
  
  int num = 1;
  for(int i = 0; i < num; ++i) {
    g2->add(new Slider<float>("Start Size", start_size, 0.1f, 50.0f, 0.01f));
    g2->add(new Slider<float>("End Size", end_size, 0.0f, 50.0f, 0.1f));
    g2->add(new Slider<float>("Begin Angle", start_angle, 0.0f, 150.0f, 0.1f));
    g2->add(new Slider<float>("End Angle", end_angle, 0.0f, 200.0f, 0.1f)).setMarginBottom(10);
  }

  g2->add(new Button("Download", 0, GUI_ICON_DOWNLOAD, on_group_button_click, NULL, GUI_CORNER_TOP));
  g2->add(new Button("Twitter", 1, GUI_ICON_TWITTER, on_group_button_click, NULL, GUI_STYLE_NONE));
  g2->add(new Button("Cloud Burst", 2, GUI_ICON_CLOUD, on_group_button_click, NULL, GUI_STYLE_NONE));
  g2->add(new Button("Extrude Region", 2, GUI_ICON_CLOUD, on_group_button_click, NULL, GUI_CORNER_BOTTOM)).setMarginBottom(10);
#endif

#if 0
  int num_els = 250;
  for (int i = 0; i < num_els; ++i) {
    char tmp_buf[1024];
    sprintf(tmp_buf, "Trail Size Alias %02d", i);
    g2->add(new Slider<int>(tmp_buf, trail_size, 0, 450, 1));
  }
#endif

  float time;
  float moo1;
  float moo2;
  Slider<float>* changing_slider = new Slider<float>("Time", time, 0.0f, 2000000.0f, 0.1f );

  g2->add(new Slider<float>("moo1", moo1, 0.0f, 2000000.0f, 0.1f));
  g2->add(new Slider<float>("moo2", moo1, 0.0f, 2000000.0f, 0.1f));
  g2->add(changing_slider);

  panel.x = 10;
  panel_ptr = &panel;
#endif

#if USE_CONTAINER
  /* Freely position elements in a container. */
  Container* container = new Container(new RenderGL());
  container->add(new Button("Download", 0, GUI_ICON_DOWNLOAD, on_group_button_click, NULL, GUI_CORNER_ALL | GUI_OUTLINE | GUI_EMBOSS)).setPosition(10, 500).setWidth(100);
  container->add(new Select("Webcam", 2, options, on_select_click, NULL, GUI_CORNER_ALL | GUI_OUTLINE | GUI_EMBOSS )).setPosition(10, 540).setWidth(200);
  container_ptr = container;
#endif


  remoxly_ptr = new Remoxly();
  /*
  remoxly_ptr->addGroup("New group");
  remoxly_ptr->addColor("Color", color, 50);
  remoxly_ptr->curr_panel->x = 350;
  */
  remoxly_ptr->load("remoxly_test.xml");

#if USE_PANEL
  float test_slider_value = 0.0f;
#endif

  while(!glfwWindowShouldClose(win)) {
    glClearColor(0.445f, 0.445f, 0.445f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if USE_GROUP
    group0.draw();
    //group1.draw();
#endif

#if USE_PANEL
# if 1
    //    test_slider->setAbsoluteValue( (0.5 + sin(test_slider_value) * 0.5) * 10.0f);
    //    test_slider->needs_redraw = true;
#endif

    panel.draw();
    //    changing_slider->setAbsoluteValue( (0.5 + sin(test_slider_value) * 0.5) * 1000000.0f);
    //    changing_slider->needs_redraw = true;
    //test_slider_value += 0.3;

#endif

#if USE_CONTAINER
    container->draw();
#endif

    remoxly_ptr->draw();

    glfwSwapBuffers(win);
    glfwPollEvents();
  }
 
  glfwTerminate();
 
  return EXIT_SUCCESS;
}
 
void char_callback(GLFWwindow* win, unsigned int key) {

  printf("CHAR: %d\n", key);

  if(group_ptr0) {
    group_ptr0->onCharPress(key);
  }

  if(group_ptr1) {
    group_ptr1->onCharPress(key);
  }

  if(panel_ptr) {
    panel_ptr->onCharPress(key);
  }

  if (container_ptr) {
    container_ptr->onCharPress(key);
  }
}
 
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {

  if(action == GLFW_RELEASE) {
    if(group_ptr0) {
      group_ptr0->onKeyRelease(key, mods);
    }
    if(group_ptr1) {
      group_ptr1->onKeyRelease(key, mods);
    }
    if(panel_ptr) {
      panel_ptr->onKeyRelease(key, mods);
    }
    if(remoxly_ptr) {
      remoxly_ptr->onKeyRelease(key, mods);
    }
    if (container_ptr) {
      container_ptr->onKeyRelease(key, mods);
    }
    return;
  }

  if( (action != GLFW_PRESS) && (action != GLFW_REPEAT) ) {
    return;
  }

  if(group_ptr0) {
    group_ptr0->onKeyPress(key, mods);
  }

  if(group_ptr1) {
    group_ptr1->onKeyPress(key, mods);
  }

  if(remoxly_ptr) {
    remoxly_ptr->onKeyPress(key, mods);
  }

  if(panel_ptr) {
    panel_ptr->onKeyPress(key, mods);
    if(key == GLFW_KEY_F1) {
      panel_ptr->needs_redraw = true;
    }
  }

  if (container_ptr) {
    container_ptr->onKeyPress(key, mods);
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

  if(group_ptr0) {
    group_ptr0->onMouseMove(x, y);
  }

  if(group_ptr1) {
    group_ptr1->onMouseMove(x, y);
  }

  if(panel_ptr) {
    panel_ptr->onMouseMove(x, y);
  }

  if(remoxly_ptr) {
    remoxly_ptr->onMouseMove(x, y);
  }

  if (container_ptr) {
    container_ptr->onMouseMove(x, y);
  }
}

void button_callback(GLFWwindow* win, int bt, int action, int mods) {

  double mx = 0;
  double my = 0;

  glfwGetCursorPos(win, &mx, &my);
  
  if(action == GLFW_PRESS) {
    if(group_ptr0) {
      group_ptr0->onMousePress(mx, my, bt, mods);
    }
    if(group_ptr1) {
      group_ptr1->onMousePress(mx, my, bt, mods);
    }
    if(panel_ptr) {
      panel_ptr->onMousePress(mx, my, bt, mods);
    }
    if(remoxly_ptr) {
      remoxly_ptr->onMousePress(mx, my, bt, mods);
    }
    if (container_ptr) {
      container_ptr->onMousePress(mx, my, bt, mods);
    }
  }
  else if(action == GLFW_RELEASE) {
    if(group_ptr0) {
      group_ptr0->onMouseRelease(mx, my, bt, mods);
    }
    if(group_ptr1) {
      group_ptr1->onMouseRelease(mx, my, bt, mods);
    }
    if(panel_ptr) {
      panel_ptr->onMouseRelease(mx, my, bt, mods);
    }
    if(remoxly_ptr) {
      remoxly_ptr->onMouseRelease(mx, my, bt, mods);
    }
    if (container_ptr) {
      container_ptr->onMouseRelease(mx, my, bt, mods);
    }
  }
}

void error_callback(int err, const char* desc) {
  printf("GLFW error: %s (%d)\n", desc, err);
}

void on_group_button_click(int id, void* user) {

  if(id == 3 && panel_ptr) {
    StorageXML xml("panel.xml");
    xml.addPanel(panel_ptr);
    xml.save();
    printf("Saved.\n");
    if(remoxly_ptr) {
      remoxly_ptr->save("remoxly_test.xml");
    }
  }
  else if(id == 4 && panel_ptr) {
    StorageXML xml("panel.xml");
    xml.addPanel(panel_ptr);
    xml.load();
    printf("Loaded.\n");
    if(remoxly_ptr) {
      remoxly_ptr->load("remoxly_test.xml");
    }
  }
}

void on_select_click(int selectid, int optionid, void* user) {
  printf("Selected an option: %d for select: %d\n", optionid, selectid);
}
