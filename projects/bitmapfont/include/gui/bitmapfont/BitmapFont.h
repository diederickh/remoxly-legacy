/*

  BitmapFont
  ----------
  BitmapFont.reset()                                                 - remove all vertex data; use this if you have changing text all the time
  BitmapFont.write(x, y, "My Text")                                  - create vertices that you later draw() at x,y
  BitmapFont.setColor(r,g,b,a)                                       - set the font color
  BitmapFont.resize(winW, winH)                                      - when your window size changes, call this
  BitmapFont.print()                                                 - print some debug info.

  This class loads fonts that are generated by the BMFont application. The 
  API for this class is optimized for performance. We follow the "page" 
  metaphore, you write on a page and then you present. That means that you 
  first need to call write once (somewhere in your setup function) and then 
  call draw(). We use this approach because we don't want to regenerate all
  the vertices everytime you render something.

  Usage
  -----

  - First you call `setup()` and pass the path to the .fnt file you got from 
    BMFont and the width and height of your window. We need the size of the 
    window so we can create a correct ortho graphic projection matrix.
  - The top left of your window is 0,0.
  - Once loaded, call `write(x,y,"text")` as often as you want. 
  - Then in your render loop, call `draw()`
  - When your screen size changes, call `resize(w,h)`

  Creating fonts with BMFont
  --------------------------
  
  We use [BMFont](http://www.angelcode.com/products/bmfont/) and you must
  follow these settings. We use fixed heights because this eases use with 
  the calculations for the vertices and may help if you want to create solid
  backgrounds for the characters. 

  These options give good results:
  
  Settings 1: 
  -------------------------------------
                Options > Font Settings:    
                
                - Size: e.g. 13px         
                - Height %: 100
                - Turn off all other checkboxes!
                
                
                Options > Export Options                                  
                   
                - Padding: all zero
                - Spacing: 1, 1
                - [x] Equalize the cell heights
                - Texture: Use a power of 2 sized texture 
                - File Format: XML
                - Textures: PNG
                - Turn off all other checkboxes!

  Settings 2:  Freetype can give pretty decent results! 
  -------------------------------------
                Options > Font Settings:    
                
                - Size: e.g. 13px         
                - Height %: 100
                - Turn off all other checkboxes!
                [x] Render from TrueType outline
                [x] TrueType hinting                     <--- USE THIS WHEN THE KERNING/SPACING SEEMS WIERD
                [x] Font Smoothing 
                
                
                Options > Export Options                                  
                   
                - Padding: all zero
                - Spacing: 1, 1
                - [x] Equalize the cell heights
                - Texture: Use a power of 2 sized texture 
                - File Format: XML
                - Textures: PNG
                - Turn off all other checkboxes!

 
  When you've set all the settings, you can create the bitmap font by 
  doing `Options > Save bitmap font as...`. Copy the .fnt and [name]_0.png 
  files to the same directory.

*/

// ------------------------------------------------------------------------------
// H E A D E R
// ------------------------------------------------------------------------------

#ifndef ROXLU_BITMAP_FONT_H
#define ROXLU_BITMAP_FONT_H

#include <string>
#include <map>
#include <vector>
#include <string.h>

// ------------------------------------------------------------------------------

#define BITMAP_FONT_ALIGN_LEFT      1
#define BITMAP_FONT_ALIGN_CENTER    2
#define BITMAP_FONT_ALIGN_RIGHT     3

#define BITMAP_FONT_GL2             1
#define BITMAP_FONT_GL3             2

#if !defined(BITMAP_FONT_GL)
#  define BITMAP_FONT_GL BITMAP_FONT_GL3
#endif

#if defined(__APPLE__) && BITMAP_FONT_GL == BITMAP_FONT_GL2
#  define glGenVertexArrays glGenVertexArraysAPPLE
#  define glBindVertexArray glBindVertexArrayAPPLE
#endif

// ------------------------------------------------------------------------------

#if BITMAP_FONT_GL == BITMAP_FONT_GL2
static const char* BITMAP_FONT_VS = ""
  "#version 110\n"
  "uniform mat4 u_pm;"
  "attribute vec4 a_pos;"
  "attribute vec2 a_tex;"
  "attribute vec4 a_fg_color; "
  "varying vec2 v_tex;"
  "varying vec4 v_fg_color;"
  ""
  "void main() {"
  "   gl_Position = u_pm * a_pos; "
  "   v_tex = a_tex;"
  "   v_fg_color = a_fg_color;"
  "}"
  "";

static const char* BITMAP_FONT_FS = ""
  "#version 110\n"
  "uniform sampler2DRect u_font_tex;"
  "varying vec2 v_tex;"
  "varying vec4 v_fg_color;"
  ""
  "void main() {"
  "  float col = texture2DRect(u_font_tex, v_tex).r;"
  "  gl_FragColor = col * v_fg_color * v_fg_color.a;"
  "}"
  "";
#endif

// ------------------------------------------------------------------------------

#if BITMAP_FONT_GL == BITMAP_FONT_GL3
static const char* BITMAP_FONT_VS = ""
  "#version 150\n"
  "uniform mat4 u_pm;"
  "in vec4 a_pos;"
  "in vec2 a_tex;"
  "in vec4 a_fg_color; "
  "out vec2 v_tex;"
  "out vec4 v_fg_color;"
  ""
  "void main() {"
  "   gl_Position = u_pm * a_pos; "
  "   v_tex = a_tex;"
  "   v_fg_color = a_fg_color;"
  "}"
  "";

static const char* BITMAP_FONT_FS = ""
  "#version 150\n"
  "uniform sampler2DRect u_font_tex;"
  "in vec2 v_tex;"
  "in vec4 v_fg_color;"
  "out vec4 fragcolor;"
  ""
  "void main() {"
  "  float col = texture(u_font_tex, v_tex).r;"
  "  fragcolor = col * v_fg_color;"
  "}"
  "";
#endif

namespace rx { 

// ------------------------------------------------------------------------------

struct Character {
  Character();

  unsigned int id;
  int x; 
  int y;
  int width;
  int height;
  int xoffset;
  int yoffset;
  int xadvance;
};

// ------------------------------------------------------------------------------

struct CharacterVertex {
  CharacterVertex();
  CharacterVertex(float x, float y, int s, int t, float* rgba);

  float x;
  float y;
  float s;
  float t;
  float fg_color[4];
};

// ------------------------------------------------------------------------------

class BitmapFont {

 public:
  BitmapFont();

  void clear();
  void write(float x, float y, std::string str, int align);    /* write a complete string */
  void write(float x, float y, unsigned int id);               /* write a specific character */
  void write(float x, float y, Character& c);                  /* write a specific character */
  void draw();
  void setColor(float r, float g, float b, float a = 1.0);
  void resize(int winW, int winH);                             /* whenever the viewport changes call this; it will recalculate the ortho matrix */
  void print(); 

  bool getChar(unsigned int code, Character& result);          /* get a specific character, returns false when the font doesn't have the char */
  float getWidth(const std::string& str);                      /* get the width for the given string, as it would be drawn on screen */

 protected:
  bool setupGraphics();
  void setupTexture(int w, int h, unsigned char* pix);
  void updateVertices();

 public:

  /* state */
  bool needs_update;
  int win_w;
  int win_h;
  float color[4];

  /* common font info */
  int line_height;
  int scale_w;
  int scale_h;
  int pages;
  int base;

  /* characters */
  std::map<unsigned int, Character> chars;

  /* opengl */
  float pm[16];  
  static GLuint prog;
  static GLuint vert;
  static GLuint frag;
  static bool is_initialized;   /* indicates if the prog, vert, frag and vao are initialized */
  GLuint vao;
  GLuint vbo;

  GLuint tex;
  size_t bytes_allocated;
  std::vector<CharacterVertex> vertices;
};

} // namespace rx

#endif

// ------------------------------------------------------------------------------
// I M P L E M E N T A T I O N
// ------------------------------------------------------------------------------

#if defined(BITMAP_FONT_IMPLEMENTATION)

namespace rx { 

GLuint BitmapFont::prog = 0;
GLuint BitmapFont::vert = 0;
GLuint BitmapFont::frag = 0;
bool BitmapFont::is_initialized = false;

// ------------------------------------------------------------------------------

static GLuint bitmapfont_create_program(GLuint vert, GLuint frag, int natts = 0, const char** atts = NULL) {
  GLuint prog = glCreateProgram();
  glAttachShader(prog, vert);
  glAttachShader(prog, frag);

  for(int i = 0; i < natts; ++i) {
    glBindAttribLocation(prog, i, atts[i]);
  }

  glLinkProgram(prog);
  return prog;
}
  
static GLuint bitmapfont_create_shader(GLenum type, const char* src) {
  GLuint s = glCreateShader(type);
  glShaderSource(s, 1, &src,  NULL);
  glCompileShader(s);
  return s;
}

static void bitmapfont_ortho(float l, float r, float b, float t, float n, float f, float* m) {
  memset((char*)m, 0x0, sizeof(float) * 16);
  float rml = r - l;
  float fmn = f - n;
  float tmb = t - b;
  m[0]  = 2.0f / rml;
  m[5]  = 2.0f / tmb;
  m[10] = -2.0f / fmn;
  m[12] = -(r+l)/rml;
  m[13] = -(t+b)/tmb;
  m[14] = -(f+n)/fmn;
  m[15] = 1.0f;
}

// -----------------------------------------------------------------------------

BitmapFont::BitmapFont()
  :needs_update(false)
  ,win_w(0)
  ,win_h(0)
  ,vbo(0)
  ,vao(0)
  ,tex(0)
  ,bytes_allocated(0)
  ,line_height(0)
  ,scale_w(0)
  ,scale_h(0)
  ,pages(0)
  ,base(0)
{
  color[0] = color[1] = color[2] = color[3] = 1.0f;
}

bool BitmapFont::setupGraphics() {

  if(!BitmapFont::is_initialized) {

    const char* atts[] = { "a_pos", "a_tex", "a_fg_color" } ;
    vert = bitmapfont_create_shader(GL_VERTEX_SHADER, BITMAP_FONT_VS);
    frag = bitmapfont_create_shader(GL_FRAGMENT_SHADER, BITMAP_FONT_FS);
    prog = bitmapfont_create_program(vert, frag, 3, atts);

    GLint viewport[4] = { 0 } ;
    glGetIntegerv(GL_VIEWPORT, viewport);
    bitmapfont_ortho(0.0f, viewport[2], viewport[3], 0.0f, 0.0f, 100.0f, pm);

    glUseProgram(prog);
    glUniformMatrix4fv(glGetUniformLocation(prog, "u_pm"), 1, GL_FALSE, pm);
    glUniform1i(glGetUniformLocation(prog, "u_font_tex"), 0);
  
    BitmapFont::is_initialized = true;
  }

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glEnableVertexAttribArray(0);  // pos
  glEnableVertexAttribArray(1);  // tex
  glEnableVertexAttribArray(2);  // fg_color

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(CharacterVertex), (GLvoid*)0); // pos
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CharacterVertex), (GLvoid*)8); // tex
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(CharacterVertex), (GLvoid*)16); // fg_color

  return true;
}

void BitmapFont::setupTexture(int w, int h, unsigned char* pix) {
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_RECTANGLE, tex);
  glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_R8, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, pix);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void BitmapFont::resize(int winW, int winH) {

  if(!winW || !winH) {
    return;
  }

  win_w = winW;
  win_h = winH;

  bitmapfont_ortho(0.0f, win_w, win_h, 0.0f, 0.0, 100.0, pm);
  glUseProgram(prog);
  glUniformMatrix4fv(glGetUniformLocation(prog, "u_pm"), 1, GL_FALSE, pm);
}

void BitmapFont::setColor(float r, float g, float b, float a) {
  color[0] = r;
  color[1] = g;
  color[2] = b;
  color[3] = a;
}

void BitmapFont::clear() {
  vertices.clear();
}


void BitmapFont::write(float x, float y, std::string str, int align) {

  float xoffset = x;
  float yoffset = y;

  if(align == BITMAP_FONT_ALIGN_RIGHT) {
    xoffset -= getWidth(str);
  }

  for(size_t i = 0; i < str.size(); ++i) {

    std::map<unsigned int, Character>::iterator it = chars.find(str[i]);
    if(it == chars.end()) {
      printf("character: %c not found.\n", str[i]);
      continue;
    }

    Character& c = it->second;
    
    write(xoffset, yoffset, c);

    xoffset += c.xadvance;
  }

}

void BitmapFont::write(float x, float y, unsigned int id)  {

  std::map<unsigned int, Character>::iterator it = chars.find(id);

  if(it == chars.end()) {
    printf("Character for id: %u not found.\n", id);
    return;
  }

  write(x, y, it->second);
}

void BitmapFont::write(float x, float y, Character& c) {

  float x0 = x + c.xoffset;
  float y0 = y + c.yoffset;
  float x1 = x0 + c.width;
  float y1 = c.height + y + c.yoffset;
  float u0 = c.x;
  float u1 = c.x + c.width;
  float v0 = c.y; 
  float v1 = v0 + c.height;

  CharacterVertex va(x0, y1, u0, v1, color); // bottom left
  CharacterVertex vb(x1, y1, u1, v1, color); // bottom right
  CharacterVertex vc(x1, y0, u1, v0, color); // top right
  CharacterVertex vd(x0, y0, u0, v0, color); // top left
    
  vertices.push_back(va);
  vertices.push_back(vb);
  vertices.push_back(vc);
  vertices.push_back(va);
  vertices.push_back(vc);
  vertices.push_back(vd);

  needs_update = true;

  //printf("x0: %f, y0: %f, x1: %f, y1: %f u0: %f, u1: %f, v0: %f, v1: %f\n", x0, y0, x1, y1, u0, u1, v0, v1);
}

void BitmapFont::draw() {

  if(!vertices.size()) {
    return;
  }

  updateVertices();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_RECTANGLE, tex);

  glUseProgram(prog);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void BitmapFont::updateVertices() {

  if(!needs_update) {
    return;
  }

  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  size_t needed = sizeof(CharacterVertex) * vertices.size();
  if(!needed) {
    return;
  }

  if(needed > bytes_allocated) {
    glBufferData(GL_ARRAY_BUFFER, needed, &vertices[0].x, GL_DYNAMIC_DRAW);
    bytes_allocated = needed;
  }
  else {
    glBufferSubData(GL_ARRAY_BUFFER, 0, needed, &vertices[0].x);
  }

  needs_update = false;
}

bool BitmapFont::getChar(unsigned int code, Character& result) {

  std::map<unsigned int, Character>::iterator it = chars.find(code);

  if(it == chars.end()) {
    return false;
  }
  
  result = it->second;

  return true;
}

float BitmapFont::getWidth(const std::string& str) {
  float w = 0.0f;
  Character ch;

  for(size_t i = 0; i < str.size(); ++i) {
    if(getChar(str[i], ch)) {
      w += ch.xadvance;
    }
  }

  return w;
}

void BitmapFont::print() {
  printf("font.line_height: %d\n", line_height);
  printf("font.scale_w: %d\n", scale_w);
  printf("font.scale_h: %d\n", scale_h);
  printf("font.pages: %d\n", pages);
  printf("font.base: %d\n", base);
}

// -----------------------------------------------------------------------------

Character::Character() 
  :id(0)
  ,x(0)
  ,y(0)
  ,width(0)
  ,height(0)
  ,xoffset(0)
  ,yoffset(0)
  ,xadvance(0)
{
}

// -----------------------------------------------------------------------------

CharacterVertex::CharacterVertex()
  :x(0.0f)
  ,y(0.0f)
  ,s(0.0f)
  ,t(0.0f)
{
  fg_color[0] = fg_color[1] = fg_color[2] = fg_color[3] = 1.0f;
}

CharacterVertex::CharacterVertex(float x, float y, int s, int t, float* rgba) 
  :x(x)
  ,y(y)
  ,s(s)
  ,t(t)
{
  fg_color[0] = rgba[0];
  fg_color[1] = rgba[1];
  fg_color[2] = rgba[2]; 
  fg_color[3] = rgba[3];
}

} // namespace rx

#endif // BITMAP_FONT_IMPLEMENTATION
