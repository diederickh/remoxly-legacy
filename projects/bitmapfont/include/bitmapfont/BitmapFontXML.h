/*

  BitmapFontXML
  -------------

  The BitmapFontXML class extends the BitmapFont class and will
  add a feature that will load the font definition that is created 
  by the BMFont utility. It uses tinylib to read the PNG that contains
  the character data.

  The BitmapFontXML file depends on tinylib, but the BitmapFont class 
  itself not. This means you can use it in any openGL application you'd
  like that has a context with 3 or higher.

  Usage:
  ------

  // Include the correct files 
  #define BITMAP_FONT_IMPLEMENTATION
  #include <bitmapfont/BitmapFont.h>
  #include <bitmapfont/BitmapFontXML.h>
   
  #define ROXLU_USE_ALL
  #define ROXLU_IMPLEMENTATION
  #include <tinylib.h>

  // setup
  BitmapFontXML font;
  if(!font.loadFile("/path/to/font.fnt")) {
    printf("Loading font failed.\n");
    ::exit(EXIT_FAILURE);
  }

  font.write(10,10, "Hello Universe!");

  // draw
  font.draw();

 */

// ------------------------------------------------------------------------------
// H E A D E R
// ------------------------------------------------------------------------------

#ifndef ROXLU_BITMAP_FONT_XML_H
#define ROXLU_BITMAP_FONT_XML_H

#define BMF_XML_CHECK(node) { if(!node) { return false; }  }

class BitmapFontXML : public BitmapFont {

 public:
  bool loadFile(std::string filepath);       /* Loads the XML and create the necessary openGL textures. Only call this when you have a correct openGL context setup, else you application will crash. */
  bool parseXML(std::string filepath);       /* Call parseXML() if you only want to parse the given xml file. This function is e.g. used by the bitmap font baker */

 protected:
  bool createTextureFromPNG(std::string filepath);

 public:
  std::string image_path;
};

#endif

// ------------------------------------------------------------------------------
// I M P L E M E N T A T I O N
// ------------------------------------------------------------------------------

#if defined(BITMAP_FONT_IMPLEMENTATION)

#include <rapidxml.hpp>
#include <sstream>

#define ROXLU_USE_PNG
#include <tinylib.h>

using namespace rapidxml;

// ------------------------------------------------------------------------------

bool BitmapFontXML::loadFile(std::string filepath) {

  if(!parseXML(filepath)) {
    return false;
  }

  if(!createTextureFromPNG(image_path)) {
    return false;
  }

  if(!setupGraphics()) {
    return false;
  }

  return true;
}

bool BitmapFontXML::parseXML(std::string filepath) {

  std::ifstream ifs(filepath.c_str(), std::ios::in);
  if(!ifs.is_open()) {
    return false;
  }
  
  std::string xml;
  xml.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

  if(!xml.size()) {
    return false;
  }

  xml_document<> xdoc;

  try {
    xdoc.parse<0>((char*)xml.c_str());
    
    // common data
    xml_node<>* xfont   = xdoc.first_node("font");                              BMF_XML_CHECK(xfont);
    xml_node<>* xcommon = xfont->first_node("common");                          BMF_XML_CHECK(xcommon);

    xml_attribute<>* xline_height = xcommon->first_attribute("lineHeight");     BMF_XML_CHECK(xline_height);
    xml_attribute<>* xscale_w     = xcommon->first_attribute("scaleW");         BMF_XML_CHECK(xscale_w);
    xml_attribute<>* xscale_h     = xcommon->first_attribute("scaleH");         BMF_XML_CHECK(xscale_h);
    xml_attribute<>* xpages       = xcommon->first_attribute("pages");          BMF_XML_CHECK(xpages);
    xml_attribute<>* xbase        = xcommon->first_attribute("base");           BMF_XML_CHECK(xbase);

    line_height = rx_to_int(xline_height->value());
    scale_w     = rx_to_int(xscale_w->value());
    scale_h     = rx_to_int(xscale_h->value());
    pages       = rx_to_int(xpages->value());
    base        = rx_to_int(xbase->value());

    // image file
    xml_node<>* xpage = xfont->first_node("pages")->first_node();               BMF_XML_CHECK(xpage);
    xml_attribute<>* xfile = xpage->first_attribute("file");                    BMF_XML_CHECK(xfile);
    std::string image_file = xfile->value();
    image_path = rx_strip_filename(filepath) +image_file;

    // character information.
    xml_node<>* xchars = xfont->first_node("chars");                           BMF_XML_CHECK(xchars);
    xml_attribute<>* xchar_count = xchars->first_attribute("count");           BMF_XML_CHECK(xchar_count);
    xml_node<>* xchar = xchars->first_node();                                  BMF_XML_CHECK(xchar);

    while(xchar) {
      xml_attribute<>* char_id       = xchar->first_attribute("id");           BMF_XML_CHECK(char_id);
      xml_attribute<>* char_x        = xchar->first_attribute("x");            BMF_XML_CHECK(char_x);
      xml_attribute<>* char_y        = xchar->first_attribute("y");            BMF_XML_CHECK(char_y);
      xml_attribute<>* char_width    = xchar->first_attribute("width");        BMF_XML_CHECK(char_width);
      xml_attribute<>* char_height   = xchar->first_attribute("height");       BMF_XML_CHECK(char_height);
      xml_attribute<>* char_xoffset  = xchar->first_attribute("xoffset");      BMF_XML_CHECK(char_xoffset);
      xml_attribute<>* char_yoffset  = xchar->first_attribute("yoffset");      BMF_XML_CHECK(char_yoffset);
      xml_attribute<>* char_xadvance = xchar->first_attribute("xadvance");     BMF_XML_CHECK(char_xadvance);

      Character c;
      c.id       = rx_to_int(char_id->value());
      c.x        = rx_to_int(char_x->value());
      c.y        = rx_to_int(char_y->value());
      c.width    = rx_to_int(char_width->value());
      c.height   = rx_to_int(char_height->value());
      c.xoffset  = rx_to_int(char_xoffset->value());
      c.yoffset  = rx_to_int(char_yoffset->value());
      c.xadvance = rx_to_int(char_xadvance->value());

      chars[c.id] = c;
      xchar = xchar->next_sibling();
    }
  }
  catch(...) {
    return false;
  }

  return true;
}


bool BitmapFontXML::createTextureFromPNG(std::string filepath) {

  if(!image_path.size()) {
    return false;
  }

  unsigned char* pix = NULL;
  int img_w = 0;
  int img_h = 0;
  int nchannels = 0;
  
  if(!rx_load_png(image_path, &pix, img_w, img_h, nchannels)) {
    return false;
  }
  
  setupTexture(img_w, img_h, pix);
 
  delete[] pix;
  pix = NULL;
  return true;
}

#endif // defined(BITMAP_FONT_IMPLEMENTATION)
