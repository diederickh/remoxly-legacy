/*

  FontBaker
  ---------

  Bakes a BitmapFont. Creates a header with the RGB + Font information 
  as static members, so you don't need any PNG loading or font parsing. 
  Execute the application from the command line. The appliation gives some
  info on what arguments it expects.

 */

#include <stdlib.h>
#include <stdio.h>

#if defined(__linux) || defined(_WIN32)
#  include <GLXW/glxw.h>
#endif

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#define ROXLU_USE_PNG
#define ROXLU_IMPLEMENTATION
#include <tinylib.h>

#define BITMAP_FONT_IMPLEMENTATION
#include <gui/bitmapfont/BitmapFont.h>
#include <gui/bitmapfont/BitmapFontXML.h>
#include <gui/fontbaker/BitmapFontBaker.h>

using namespace rx;

int main(int argc, char** argv) {

  printf("\nFontBaker v.0.0.0.0.1\n");
  printf("-----------------------\n");

  if(argc != 3) {
    printf("Usage: ./fontbaker fontfile.fnt ClassName\n");
    printf("---\n");
    printf("- The `fontfile.fnt` should be put in the data dir in install/[]/bin/. This is the file that BMFont created.\n");
    printf("- The `ClassName` is the name of the C++ class we create. Also saved in the data dir.\n");
    printf("---\n");
    ::exit(EXIT_FAILURE);
  }

  BitmapFontBaker baker;
  std::string output(rx_to_data_path(argv[2]) +".h");
  std::string classname(argv[2]);

  if(!baker.bake(classname, rx_to_data_path(argv[1]), output, BAKE_MODE_EMBED_RGB)) {
    printf("Cannot bake the font.\n");
    ::exit(EXIT_FAILURE);
  }
 
  printf("Created: %s\n", output.c_str());
  printf("\n");
  return EXIT_SUCCESS;
}
