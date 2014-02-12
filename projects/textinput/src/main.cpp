/*

  TextInput
  ---------

  Shows a simple example of how to use the text input.

  - Press F1 to select all all text. Use LEFT/RIGHT arrow to jump around, 
    BACKSPACE and DEL to delete the text.
  - Press F2 to align the text to the left.
  - Press F3 to align the text to the right.

 */
#include <stdlib.h>
#include <stdio.h>

#if defined(__linux) || defined(_WIN32)
#  include <GLXW/glxw.h>
#endif
 
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#define TEXT_INPUT_IMPLEMENTATION
#define BITMAP_FONT_IMPLEMENTATION
#include <bitmapfont/BitmapFont.h>
#include <textinput/TextInput.h>
#include "FreePixel.h"

using namespace rx; 

TextInput* text_input_ptr = NULL;

#define ROXLU_USE_ALL
#define ROXLU_IMPLEMENTATION
#include <tinylib.h>
 
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
 
  win = glfwCreateWindow(w, h, "TextInput", NULL, NULL);
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
  FreePixel free_pixel_font;
  if(!free_pixel_font.setup()) {
    printf("Cannot initialize the free pixel font.\n");
    ::exit(EXIT_FAILURE);
  }


  TextInput input(300, 10, 500, free_pixel_font);
  free_pixel_font.setColor(1.0, 1.0, 1.0, 1.0);
  ///input.align = BITMAP_FONT_ALIGN_RIGHT;
  input.align = BITMAP_FONT_ALIGN_LEFT;
  text_input_ptr = &input;

  input.enable();

  while(!glfwWindowShouldClose(win)) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    input.draw();

    glfwSwapBuffers(win);
    glfwPollEvents();
  }
 
  glfwTerminate();
 
  return EXIT_SUCCESS;
}
 
void char_callback(GLFWwindow* win, unsigned int key) {
  if(text_input_ptr) {
    text_input_ptr->onCharPress(key);
  }
}
 
void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods) {
  
  if( (action != GLFW_PRESS) && (action != GLFW_REPEAT) ) {
    return;
  }

  if(text_input_ptr) {
    text_input_ptr->onKeyPress(key, mods);
  }

  switch(key) {
    case GLFW_KEY_ESCAPE: {
      glfwSetWindowShouldClose(win, GL_TRUE);
      break;
    }
    case GLFW_KEY_F1: {
      if(text_input_ptr) {
        text_input_ptr->select();
      }
      break;
    }
    case GLFW_KEY_F2: {
      if(text_input_ptr) {
        text_input_ptr->align = BITMAP_FONT_ALIGN_LEFT;
      }
      break;
    }
    case GLFW_KEY_F3: {
      if(text_input_ptr) {
        text_input_ptr->align = BITMAP_FONT_ALIGN_RIGHT;
      }
      break;
    }
  };
}
 
void resize_callback(GLFWwindow* window, int width, int height) {
}

void cursor_callback(GLFWwindow* win, double x, double y) {
}

void button_callback(GLFWwindow* win, int bt, int action, int mods) {
}

void error_callback(int err, const char* desc) {
  printf("GLFW error: %s (%d)\n", desc, err);
}

