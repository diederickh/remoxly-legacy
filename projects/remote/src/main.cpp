#include <stdlib.h>
#include <stdio.h>

#if defined(__linux) || defined(_WIN32)
#  include <GLXW/glxw.h>
#endif
 
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include <gui/Remoxly.h>
#include <gui/remote/Remote.h>
#include <app/RemoteClientApp.h>

RemoteClientApp* app_ptr = NULL;

// The GLFW callbacks we handle.
void button_callback(GLFWwindow* win, int bt, int action, int mods);
void cursor_callback(GLFWwindow* win, double x, double y);
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods);
void char_callback(GLFWwindow* win, unsigned int key);
void error_callback(int err, const char* desc);
void resize_callback(GLFWwindow* window, int width, int height);

// ---------------------------------------------------
 
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
 
  win = glfwCreateWindow(w, h, "Remoxly Control ", NULL, NULL);
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
 
#if defined(__linux)  || defined(_WIN32)
  if(glxwInit() != 0) {
    printf("Error: cannot initialize glxw.\n");
    ::exit(EXIT_FAILURE);
  }
#endif
 
  // ----------------------------------------------------------------
  // THIS IS WHERE YOU START CALLING OPENGL FUNCTIONS, NOT EARLIER!!
  // ----------------------------------------------------------------
  RemoteClientApp app("127.0.0.1", 2255);
  app_ptr = &app;

  while(!glfwWindowShouldClose(win)) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    app.update();
    app.draw();

    glfwSwapBuffers(win);
    glfwPollEvents();
  }
 
  glfwTerminate();
 
  return EXIT_SUCCESS;
}
 
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {

  if(app_ptr) {
    if(action & GLFW_PRESS || action & GLFW_REPEAT) {
      app_ptr->onKeyPress(key, mods);
    }
    else if(action & GLFW_RELEASE) {
      app_ptr->onKeyRelease(key, mods);
    }
  }

  switch(key) {
    case GLFW_KEY_ESCAPE: {
      glfwSetWindowShouldClose(win, GL_TRUE);
      break;
    }
  };
}

void char_callback(GLFWwindow* win, unsigned int key) {

  if(app_ptr) {
    app_ptr->onCharPress(key);
  }
}
 
void resize_callback(GLFWwindow* window, int width, int height) {
}

void cursor_callback(GLFWwindow* win, double x, double y) {
  
  if(app_ptr) {
    app_ptr->onMouseMove(x, y);
  }
}

void button_callback(GLFWwindow* win, int bt, int action, int mods) {

  if(app_ptr) {

    double mx, my;
    glfwGetCursorPos(win, &mx, &my);

    if(action == GLFW_PRESS) {
      app_ptr->onMousePress(mx, my, bt, mods);
    }
    else {
      app_ptr->onMouseRelease(mx, my, bt, mods);
    }
  }
}

void error_callback(int err, const char* desc) {
  printf("GLFW error: %s (%d)\n", desc, err);
}

// ------------------------------------------------------------------------

