/*

  FontBaker
  ---------

  Bakes a BitmapFont. Creates a header with the RGB + Font information 
  as static members, so you don't need any PNG loading or font parsing. 

 */

#include <stdlib.h>
#include <stdio.h>

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#define ROXLU_USE_PNG
#define ROXLU_IMPLEMENTATION
#include <tinylib.h>

#define BITMAP_FONT_IMPLEMENTATION
#include <bitmapfont/BitmapFont.h>
#include <bitmapfont/BitmapFontXML.h>
#include <fontbaker/BitmapFontBaker.h>

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
