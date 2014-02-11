/*

  This file contains a minimal example on how to use Remoxly.
  We create a panel with two Groups

 */
#include <stdlib.h>
#include <stdio.h>

#if defined(__linux)
#  include <GLXW/glxw.h>
#endif
 
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

// Include the GUI types:
#include <gui/Remoxly.h>
#include <gui/remote/Client.h>

// After we included the correct GL headers which 
// is done by the above glfw3.h include, we can include
// the GL implementation.
#include <gui/gl/ImplementationGL.h>

#define ROXLU_USE_PNG
#define ROXLU_USE_OPENGL
#define ROXLU_IMPLEMENTATION
#include <tinylib.h>

// Because we want to access the Panel we create below,
// we store a pointer to it int panel_ptr.
Panel* panel_ptr = NULL;

// Our button callback function
void button_click(int id, void* user);
 
// The GLFW callbacks we handle.
void button_callback(GLFWwindow* win, int bt, int action, int mods);
void cursor_callback(GLFWwindow* win, double x, double y);
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods);
void char_callback(GLFWwindow* win, unsigned int key);
void error_callback(int err, const char* desc);
void resize_callback(GLFWwindow* window, int width, int height);
 
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
 
  win = glfwCreateWindow(w, h, "Remoxly GUI ", NULL, NULL);
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
 
#if defined(__linux)  
  if(glxwInit() != 0) {
    printf("Error: cannot initialize glxw.\n");
    ::exit(EXIT_FAILURE);
  }
#endif
 
  // ----------------------------------------------------------------
  // THIS IS WHERE YOU START CALLING OPENGL FUNCTIONS, NOT EARLIER!!
  // ----------------------------------------------------------------
  int num_particles = 10;
  float speed = 5.5f;
  float trail_width = 10.0f;
  int trail_length = 10;
  bool render_particles = false;
  bool render_trails = true;
  bool render_effects = false;
  bool spawn_particles = false;
  float particle_color[3] = { 0.0f } ;
  float trail_color[3] = { 0.0f } ;
  int num_files = 5;
  std::string name = "";
  std::string filename = "";
  GLuint tex_id = rx_create_texture(rx_to_data_path("texture.png"));

  // create a panel with a height of 300 pixels. A panel 
  // is a collection of Groups. You can add as many as Groups
  // to your panel.
  Panel panel(new RenderGL(), 500);       

  // Set the panel_ptr, that is used in the GLFW callbacks
  panel_ptr = &panel;

  // Create a new Group for our particle settings.
  Group* particle_group = panel.addGroup("Particles");
  //  particle_group->add(new Text("Server", name));
  //  particle_group->add(new Text("Port", name));
  particle_group->add(new ColorRGB("Particle Color", particle_color));
  particle_group->add(new ColorRGB("Trail color", trail_color, 15, 0.5f)); // create 15 color selection; and a saturation level of 0.5f
  particle_group->add(new Slider<float>("Trail Width", trail_width, 0.0f, 100.0f, 0.1f)); // range: [0.0f - 100.0f], step: 0.1f
  particle_group->add(new Slider<float>("Speed", speed, 0.0f, 50.0f, 0.01)); 
  particle_group->add(new Slider<int>("Number Of Particles", num_particles, 0, 500, 1));
  particle_group->add(new Slider<int>("Trail Length", trail_length, 0, 100, 2));
  particle_group->add(new Toggle("Render Particles", render_particles));
  particle_group->add(new Toggle("Render Trails", render_trails));
  particle_group->add(new Toggle("Render Effects", render_effects));
  particle_group->add(new Toggle("Spawn Particles", spawn_particles));

  Group* storage_group = panel.addGroup("Save and Load");
  storage_group->add(new Text("Filename", filename, 200));
  storage_group->add(new Texture("Blob", new TextureInfoGL(GL_TEXTURE_2D, tex_id)));
  storage_group->add(new Slider<int>("Number of files", num_files, 0, 15, 1));
  storage_group->add(new Button("Save", 0, GUI_ICON_FLOPPY_O, button_click));
  storage_group->add(new Button("Load", 1, GUI_ICON_REFRESH, button_click));

  Client client("127.0.0.1", 2255, false);
  client.addPanel(&panel);
  client.connect();
  
  while(!glfwWindowShouldClose(win)) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw the panel and all it's containing widgets
    client.update();
    panel.draw();
    
    glfwSwapBuffers(win);
    glfwPollEvents();
  }
 
  glfwTerminate();
 
  return EXIT_SUCCESS;
}
 
void char_callback(GLFWwindow* win, unsigned int key) {

  // Pass the ascii characters to the panel.
  if(panel_ptr) {
    panel_ptr->onCharPress(key);
  }
}
 
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
  
  // Call onKeyRelease and onKeyPress and pass the key and modificator keys
  if(action == GLFW_RELEASE && panel_ptr) {
    panel_ptr->onKeyRelease(key, mods);
  }
  else if(action == GLFW_PRESS && panel_ptr) {
    panel_ptr->onKeyPress(key, mods);
  }

  switch(key) {
    case GLFW_KEY_ESCAPE: {
      glfwSetWindowShouldClose(win, GL_TRUE);
      break;
    }
  };
}
 
void resize_callback(GLFWwindow* window, int width, int height) {

  // when we resize the window, we need to pass the w/h to our render object
  if(panel_ptr) {
    panel_ptr->render->resize(width, height);
  }
}

void cursor_callback(GLFWwindow* win, double x, double y) {

  // Tell the panel where our cursor is located
  if(panel_ptr) {
    panel_ptr->onMouseMove(x, y);    
  }
}

void button_callback(GLFWwindow* win, int bt, int action, int mods) {

  double mx = 0;
  double my = 0;
  
  glfwGetCursorPos(win, &mx, &my);

  // Forward mouse clicks
  if(action == GLFW_PRESS && panel_ptr) {
    panel_ptr->onMousePress(mx, my, bt, mods);
  }
  else if(action == GLFW_RELEASE && panel_ptr) {
    panel_ptr->onMouseRelease(mx, my, bt, mods);
  }
}

void error_callback(int err, const char* desc) {
  printf("GLFW error: %s (%d)\n", desc, err);
}

/*

  This is the function that we use to handle button clicks. We 
  pass a unique ID to each button so we can use one function to 
  handle multiple buttons. The ID is passed into this function 
  together with the user pointer we gave it. 

 */
void button_click(int id, void* user) {

  if(id == 0) {
    printf("Save.\n");
    StorageXML xml("settings.xml");
    xml.addPanel(panel_ptr);
    xml.save();
  }
  else if(id == 1) {
    printf("Load.\n");
    StorageXML xml("settings.xml");
    xml.addPanel(panel_ptr);
    xml.load();
  }
}
