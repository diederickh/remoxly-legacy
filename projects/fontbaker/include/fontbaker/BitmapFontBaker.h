/*

  BitmapFont Baker.
  -----------------

  Generates a header for a font for the BitmapFont class. 
  It will store the raw RGB and XML data in static variables.
  These variables are used to create the texture and for the 
  character sizes (alignment).

 */

// ------------------------------------------------------------------------------
// H E A D E R
// ------------------------------------------------------------------------------

#ifndef ROXLU_BITMAP_FONT_BAKER_H
#define ROXLU_BITMAP_FONT_BAKER_H

#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>
#include <stdint.h>

// --------------------------------------------------

#define BAKE_MODE_EMBED_RGB 2
#define BAKE_USE_MAX_WIDTH 0

// --------------------------------------------------

class BitmapFontBaker {
 public:
  BitmapFontBaker();
  bool bake(std::string classname, std::string filepath, std::string destpath, int mode = BAKE_MODE_EMBED_RGB);
};

#endif


// ------------------------------------------------------------------------------
// I M P L E M E N T A T I O N
// ------------------------------------------------------------------------------
#if defined(BITMAP_FONT_IMPLEMENTATION)

BitmapFontBaker::BitmapFontBaker() {
}

bool BitmapFontBaker::bake(std::string classname, std::string filepath, std::string destpath, int mode) {

  if(!classname.size()) {
    return false;
  }

  BitmapFontXML font;

  if(!font.parseXML(filepath)) {
    printf("Cannot find file: %s\n", filepath.c_str());
    return false;
  }

  // Serialize the character data
  // --------------------------------------------------
#if BAKE_USE_MAX_WIDTH
  int max_col = 180;
  int col = 0;
#endif

  int char_count = 0;
  std::string char_data; 
  
  char_data = "{";
  
  std::map<unsigned int, Character>::iterator it = font.chars.begin(); 
  
  while(it != font.chars.end()) {

    Character& c = it->second;

    std::stringstream ss;

    ss << c.id       << "," 
       << c.x        << "," 
       << c.y        << "," 
       << c.width    << "," 
       << c.height   << ","
       << c.xoffset  << ","
       << c.yoffset  << ","
       << c.xadvance << "";
    
    ++it;
    ++char_count;

    if(it != font.chars.end()) {
      ss << ",";
    }
    
    std::string line = ss.str();
    char_data += line;

#if BAKE_USE_MAX_WIDTH

    col += line.size();

    if(col > max_col) {
      col = 0;
      char_data += "";
    }
#endif
  }

  char_data += "};\n";


  // Serialize the general font info
  // --------------------------------------------------

  std::string font_info;

  {
    std::stringstream ss;
    ss << "    line_height = " << font.line_height << ";\n";
    ss << "    scale_w = " << font.scale_w << ";\n";
    ss << "    scale_h = " << font.scale_h << ";\n";
    ss << "    pages = " << font.pages << ";\n";
    ss << "    base = " << font.base << ";\n";
    ss << "\n";
    ss << "    int num_chars = " << char_count << ";\n";
    ss << "    int max_els = num_chars * 8;\n";
    ss << "    int i = 0;\n";
      
    font_info = ss.str();
  }

  // Embed the binary data
  // --------------------------------------------------
  std::string pixel_data;
  {
    unsigned char* pix;
    int w, h, c;

    if(!rx_load_png(font.image_path, &pix, w, h, c)) {
      return false;
    }
    
    uint64_t* ptr = (uint64_t*)pix;
    long int num_bytes = ((w * c) * (h * c));
    long int num_ints = num_bytes / sizeof(uint64_t);

    pixel_data = "{";

    for(int i = 0; i < num_ints; ++i) {

      std::stringstream hex_data;
      hex_data << "0x" << std::hex << ptr[i];

      if(i < (num_ints - 1)) {
        hex_data << ",";
      }

      std::string line = hex_data.str();
      pixel_data += line;

      
#if BAKE_USE_MAX_WIDTH

      col += line.size();

      if(col > max_col) {
        col = 0;
        pixel_data +="\n";
      }
#endif

    }
    pixel_data += "};";
  }

  // Write the header
  // --------------------------------------------------
  {
    // variable name.
    std::string varname = classname;
    std::string uppername = classname;

    std::transform(varname.begin(), varname.end(), varname.begin(), ::tolower);
    std::transform(uppername.begin(), uppername.end(), uppername.begin(), ::toupper);
    
    // create code
    std::stringstream ss;
    ss << "#ifndef " << uppername << "_BAKED_H\n"
       << "#define " << uppername << "_BAKED_H\n"
       << "\n"
       << "static uint64_t " << varname << "_pixel_data[] = " << pixel_data << "\n"
       << "static int " << varname << "_char_data[] = " << char_data << "\n"
       << "";
       
    ss << "class " << classname << " : public BitmapFont {\n"
       << "public:\n"
       << "\n"
       << "  bool setup() { \n"
       << "\n"
       << "    if(!setupGraphics()) {\n"
       << "       return false;\n"
       << "    }\n"
       << "\n"
       << font_info
       << "\n"
       << "    while(i < max_els) {\n"
       << "      Character c;\n"
       << "      c.id       = " << varname << "_char_data[i++];\n"
       << "      c.x        = " << varname << "_char_data[i++];\n"
       << "      c.y        = " << varname << "_char_data[i++];\n"
       << "      c.width    = " << varname << "_char_data[i++];\n"
       << "      c.height   = " << varname << "_char_data[i++];\n"
       << "      c.xoffset  = " << varname << "_char_data[i++];\n"
       << "      c.yoffset  = " << varname << "_char_data[i++];\n"
       << "      c.xadvance = " << varname << "_char_data[i++];\n"
       << "      chars[c.id] = c;\n"
       << "    }\n"
       << "\n"
       << "    setupTexture(scale_w, scale_h, (unsigned char*)&" << varname << "_pixel_data[0]);\n"
       << "\n"
       << "    return true;\n"
       << "  }\n"
       << "};\n"
       << "\n"
       << "#endif\n"
       << "";
    
    // write contents to header file.
    std::ofstream ofs(destpath.c_str(), std::ios::out);

    if(!ofs.is_open()) {
      printf("Error: cannot open the out file.\n");
      return false;
    }

    std::string class_contents = ss.str();
    ofs.write((char*)class_contents.c_str(), class_contents.size());
    ofs.close();
  }

  return true;
}

#endif // BITMAP_FONT_IMPLEMENTATION
