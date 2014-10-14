#include <stdlib.h>
#include <stdio.h>

#include "App.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define ROXLU_USE_OPENGL
#define ROXLU_IMPLEMENTATION
#include <tinylib.h>

#define REMOXLY_USE_OPENGL
#define REMOXLY_IMPLEMENTATION
#include <gui/Remoxly.h>
 
void button_callback(GLFWwindow* win, int bt, int action, int mods);
void cursor_callback(GLFWwindow* win, double x, double y);
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods);
void char_callback(GLFWwindow* win, unsigned int key);
void error_callback(int err, const char* desc);
void resize_callback(GLFWwindow* window, int width, int height);

App app;

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
 
  win = glfwCreateWindow(w, h, "TestApp", NULL, NULL);
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
  app.init();

  while(!glfwWindowShouldClose(win)) {
    glClearColor(0.445f, 0.445f, 0.445f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    app.draw();

    glfwSwapBuffers(win);
    glfwPollEvents();
  }
 

  glfwTerminate();
 
  return EXIT_SUCCESS;
}
 
void char_callback(GLFWwindow* win, unsigned int key) {
  app.container->onCharPress(key);
}
 
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
  
  if(action == GLFW_RELEASE) {
    app.container->onKeyRelease(key, mods);
  }

  if( (action != GLFW_PRESS) && (action != GLFW_REPEAT) ) {
    return;
  }

  app.container->onKeyPress(key, mods);

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
  app.container->onMouseMove(x,  y);
}

void button_callback(GLFWwindow* win, int bt, int action, int mods) {

  double mx = 0;
  double my = 0;

  glfwGetCursorPos(win, &mx, &my);
  
  if(action == GLFW_PRESS) {
    app.container->onMousePress(mx, my, bt, mods);
  }
  else if(action == GLFW_RELEASE) {
    app.container->onMouseRelease(mx, my, bt, mods);
  }
}

void error_callback(int err, const char* desc) {
  printf("GLFW error: %s (%d)\n", desc, err);
}
