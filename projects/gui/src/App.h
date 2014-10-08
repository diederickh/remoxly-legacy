/* 
   
   App
   ---
   This example shows how you can use remoxly in an "application" like class. 
   It shows how you can can set defines that are necessary to use the GL renderer.

   In the files that want to use Remoxly types, use: 
   -------------------------------------------------

        1. First make sure that you include your opengl headers
        2. `#define REMOXLY_USE_OPENGL`
        3. `#include <gui/Remoxly.h>

   In your main.cpp, use:
   ----------------------

        #define REMOXLY_IMPLEMENTATION
        #define REMOXLY_USE_OPENGL
        #include <gui/Remoxly.h>

 */
#ifndef APPLICATION_H
#define APPLICATION_H

#define REMOXLY_USE_OPENGL
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <gui/Remoxly.h>

using namespace rx;

class App {
 public:
  App();
  ~App();
  int init();
  void draw();

 public:
  Container* container;
  bool flag0;
  bool flag1;
  bool flag2;
  bool flag3;
}; 

#endif
