
// ------------------------------------------------------------------------------
// H E A D E R
// ------------------------------------------------------------------------------

#ifndef REMOXLY_RENDER_GL_H
#define REMOXLY_RENDER_GL_H

#define TEXT_INPUT_IMPLEMENTATION
#define BITMAP_FONT_IMPLEMENTATION
#include <bitmapfont/BitmapFont.h>
#include <textinput/TextInput.h>

/* @todo - pick a font in RenderGL header */
#include <gui/fonts/FreePixel.h>
#include <gui/fonts/FontAwesome.h>
#include <gui/fonts/SourceCode.h>
#include <gui/fonts/Arial.h>

#include <gui/Render.h>
#include <assert.h>

// -------------------------------------------

static const char* GUI_RENDER_VS = ""
  "#version 150\n"
  "uniform mat4 u_pm;"
  "in vec4 a_pos;"
  "in vec4 a_color;"
  "out vec4 v_color;"
  "void main() {"
  "  gl_Position = u_pm * a_pos;"
  "  v_color = a_color;"
  "}"
  "";

static const char* GUI_RENDER_FS = ""
  "#version 150\n"
  "in vec4 v_color;"
  "out vec4 fragcolor;"
  "void main() {"
  "  fragcolor = v_color;"
  "}"
  "";

// -------------------------------------------

struct GuiVertex {

public:
  GuiVertex(float x = 0.0f, float y = 0.0f, float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) { setPos(x, y); setColor(r,g,b,a); } 
  GuiVertex(float x, float y, float* col) { setPos(x, y); color[0] = col[0]; color[1] = col[1]; color[2] = col[2]; color[3] = col[3]; } 
  void setPos(float x, float y) { pos[0] = x; pos[1] = y; } 
  void setColor(float r, float g, float b, float a) { color[0] = r; color[1] = g; color[2] = b; color[3] = a; } 
  const float* ptr() { return &pos[0]; } 
  void print();

public:
  float pos[2];
  float color[4];
};

// -------------------------------------------

GLuint gui_create_shader(GLenum type, const char* src);
GLuint gui_create_program(GLuint vert, GLuint frag, int natts = 0, const char** atts = NULL);
void gui_print_program_link_info(GLuint prog);
void gui_print_shader_compile_info(GLuint shader);
void gui_ortho(float l, float r, float b, float t, float n, float f, float* dest);

// -------------------------------------------

class RenderGL : public Render {

 public:
  RenderGL();
  bool setup();
  void update();
  void draw();

  void getWindowSize(int& ww, int& wh);
  void beginScissor();
  void scissor(int sx, int sy, int sw, int sh);
  void endScissor();

  void clear();                                                                             /* removes all vertices, and offset data */
  void writeText(float x, float y, std::string text, float* color);                         /* write some text */
  void writeNumber(float x, float y, std::string number, float* color);                     /* write a number value; we right align it */
  void writeIcon(float x, float y, unsigned int icon, float* color);                        /* write an icon. see Types.h for the available icons */
  void addRectangle(float x, float y, float w, float h, float* color, bool filled = true);  /* draw a rectangle at x/y with w/h and given color; must have 4 elements */

  void enableTextInput(float x, float y, float maxw, std::string value, float* color); 
  void enableNumberInput(float x, float y, float maxw, std::string value, float* color);
  void disableNumberInput();
  void getNumberInputValue(std::string& result);
  bool getIconSize(unsigned int id, int& w, int& h);

  void onCharPress(unsigned int key); /* gets called when a key is pressed (called by an editable widget) */
  void onKeyPress(int key, int mods); /* gets called for special keys (called by an editable widget) */

 public:

  /* opengl */
  GLint viewport[4];                                           /* contains the viewport size; is e.g. used in getWindowSize() */
  GLuint vbo;                                                  /* the vbo that keeps the vertices */
  GLuint vao;                                                  /* vao to keep state of the vbo */
  static GLuint prog;                                          /* the shader program that we use to render everything */
  static GLuint vert;                                          /* the vertex shader for the gui */
  static GLuint frag;                                          /* the fragment shader for the gui */
  static bool is_initialized;                                  /* static member, is set to true when the shaders/prog has been created */

  /* buffers */
  bool needs_update;                                           /* set to true whenever we need to update the vbo */
  size_t bytes_allocated;                                      /* how many bytes we've allocated in the vbo */
  std::vector<GuiVertex> vertices;                             /* vertices for that make up the gui */
  std::vector<GLint> bg_offsets;                               /* offsets of the different background elements */
  std::vector<GLsizei> bg_counts;                              /* vertex counts for the background elements */
  std::vector<GLint> fg_offsets;                               /* offsets of the foreground elements */
  std::vector<GLsizei> fg_counts;                              /* vetex counts for the foreground elements */

  /* fonts */
  Arial text_font;
  SourceCode number_font;
  FontAwesome icon_font;
  Arial text_input_font;                                       /* we need to use another font object for the text input because the text input clears all vertices, so it cannot share the text_font. */
  SourceCode number_input_font;                                /* we need to use another font object for the number font because the number font clears all vertices, so it cannot share the number_font. */
  TextInput text_input;
  TextInput number_input;
};

#endif

// ------------------------------------------------------------------------------
// I M P L E M E N T A T I O N
// ------------------------------------------------------------------------------

#if defined(REMOXLY_IMPLEMENTATION)

// -------------------------------------------

GLuint RenderGL::prog = 0;
GLuint RenderGL::vert = 0;
GLuint RenderGL::frag = 0;
bool RenderGL::is_initialized = false;

// -------------------------------------------

RenderGL::RenderGL() 
  :vbo(0)
  ,vao(0)
  ,bytes_allocated(0)
  ,needs_update(false)
  ,text_input(0.0f, 0.0f, 0.0f, text_input_font)
  ,number_input(0.0f, 0.0f, 0.0f, number_input_font)
{
  viewport[0] = viewport[1] = viewport[2] = viewport[3] = 0;
}

bool RenderGL::setup() {

  if(!is_initialized) {

    // shader
    const char* atts[] = { "a_pos", "a_color" };
    vert = gui_create_shader(GL_VERTEX_SHADER, GUI_RENDER_VS);
    frag = gui_create_shader(GL_FRAGMENT_SHADER, GUI_RENDER_FS);
    prog = gui_create_program(vert, frag, 2, atts);

    // ortho projection 
    float pm[16];
    glGetIntegerv(GL_VIEWPORT, viewport);
    gui_ortho(0.0f, viewport[2], viewport[3], 0.0f, 0.0f, 100.0f, pm);

    glUseProgram(prog);
    glUniformMatrix4fv(glGetUniformLocation(prog, "u_pm"), 1, GL_FALSE, pm);

    is_initialized = true;
  }

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glEnableVertexAttribArray(0); // pos
  glEnableVertexAttribArray(1); // color

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GuiVertex), (GLvoid*)0); // pos
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GuiVertex), (GLvoid*)8); // pos

  if(!text_font.setup()) {
    printf("Error: cannot setup text font.\n");
    return false;
  }

  if(!icon_font.setup()) {
    printf("Error: cannot setup icon font.\n");
    return false;
  }

  if(!number_font.setup()) {
    printf("Error: cannot setup number font.\n");
    return false;
  }

  if(!text_input_font.setup()) {
    printf("Error: cannot setup the text input font.\n");
    return false;
  }

  if(!number_input_font.setup()) {
    printf("Error: cannot setup the number input font.\n");
    return false;
  }

  number_input.align = BITMAP_FONT_ALIGN_RIGHT;
  text_input.align = BITMAP_FONT_ALIGN_LEFT;
  return true;
}

void RenderGL::getWindowSize(int& ww, int& wh) {
  ww = viewport[2];
  wh = viewport[3];
}

void RenderGL::update() {

  if(!vertices.size()) {
    return;
  }
  
  if(!needs_update) {
    return;
  }

  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  size_t needed = vertices.size() * sizeof(GuiVertex);

  if(needed > bytes_allocated) {
    glBufferData(GL_ARRAY_BUFFER, needed, vertices[0].ptr(), GL_STREAM_DRAW);
    bytes_allocated = needed;
  }
  else {
    glBufferSubData(GL_ARRAY_BUFFER, 0, needed, vertices[0].ptr());
  }

  needs_update = false;
}

void RenderGL::draw() {

  if(!vertices.size()) {
    return;
  }

  glEnable(GL_BLEND);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glUseProgram(prog);
  glBindVertexArray(vao);
  glMultiDrawArrays(GL_TRIANGLES, &bg_offsets[0], &bg_counts[0], bg_counts.size());
  glMultiDrawArrays(GL_LINE_STRIP, &fg_offsets[0], &fg_counts[0], fg_counts.size());

  glBlendFunc(GL_ONE, GL_ONE);
  icon_font.draw();
  text_font.draw();
  number_font.draw();
  text_input.draw();
  number_input.draw();

  glDisable(GL_BLEND);
}


void RenderGL::beginScissor() {
  glEnable(GL_SCISSOR_TEST);
}

void RenderGL::scissor(int sx, int sy, int sw, int sh) {
  glScissor(sx, sy, sw, sh);
}

void RenderGL::endScissor() {
  glDisable(GL_SCISSOR_TEST);
}

void RenderGL::clear() {

  vertices.clear();
  bg_offsets.clear();
  bg_counts.clear();
  fg_offsets.clear();
  fg_counts.clear();
  
  icon_font.clear();
  text_font.clear();
  number_font.clear();
  text_input_font.clear();
  number_input_font.clear();

}

void RenderGL::onCharPress(unsigned int key) {

  if(text_input.mode != TI_MODE_DISABLED) {
    text_input.onCharPress(key);
  }

  if(number_input.mode != TI_MODE_DISABLED) {
    number_input.onCharPress(key);
  }
}

void RenderGL::onKeyPress(int key, int mods) {

  if(text_input.mode != TI_MODE_DISABLED) {
    text_input.onKeyPress(key, mods);
  }

  if(number_input.mode != TI_MODE_DISABLED) {
    number_input.onKeyPress(key, mods);
  }
}

void RenderGL::writeText(float x, float y, std::string text, float* color) {

  text_font.setColor(color[0], color[1], color[2], color[3]);
  text_font.write(x, y, text, BITMAP_FONT_ALIGN_LEFT);
}

void RenderGL::writeNumber(float x, float y, std::string number, float* color) {

  number_font.setColor(color[0], color[1], color[2], color[3]);
  number_font.write(x, y, number, BITMAP_FONT_ALIGN_RIGHT);
}

void RenderGL::writeIcon(float x, float y, unsigned int icon, float* color) {
  
  icon_font.setColor(color[0], color[1], color[2], color[3]);
  icon_font.write(x, y, icon);
}

void RenderGL::enableTextInput(float x, float y, float maxw, std::string value, float* color) {

  text_input.x = x;
  text_input.y = y;
  text_input.w = maxw;

  text_input.clear(); // @todo this might be a duplicate call, when a widget makes its state editable RenderGL::clear() will be called too.
  text_input.enable();
  text_input.setValue(value);
  text_input.select();
}

void RenderGL::disableNumberInput() {
  number_input.clear();
  number_input.disable();
}

void RenderGL::getNumberInputValue(std::string& result) {
  result = number_input.getValue();
}

void RenderGL::enableNumberInput(float x, float y, float maxw, std::string value, float* color) {
  number_input.x = x;
  number_input.y = y;
  number_input.w = maxw;
  
  number_input.clear();
  number_input.enable();
  number_input.setValue(value);
  number_input.select();
}

bool RenderGL::getIconSize(unsigned int id, int& ww, int& hh) {

  Character result;
  ww = 0;
  hh = 0;

  if(!icon_font.getChar(id, result)) {
    return false;
  }

  ww = result.width;
  hh = result.height;

  return true;
}

void RenderGL::addRectangle(float x, float y, float w, float h, float* color, bool filled) {

  GuiVertex a(x, y + h, color);     // bottom left
  GuiVertex b(x + w, y + h, color); // bottom right
  GuiVertex c(x + w, y, color);     // top right 
  GuiVertex d(x, y, color);         // top left
  
  if(filled) { 
    bg_offsets.push_back(vertices.size());

    vertices.push_back(a);
    vertices.push_back(b);
    vertices.push_back(c);

    vertices.push_back(a);
    vertices.push_back(c);
    vertices.push_back(d);

    bg_counts.push_back(vertices.size() - bg_offsets.back());
  }
  else {
    fg_offsets.push_back(vertices.size());
    vertices.push_back(a);
    vertices.push_back(b);
    vertices.push_back(c);
    vertices.push_back(d);
    vertices.push_back(a);
    fg_counts.push_back(vertices.size() - fg_offsets.back());
  }
  needs_update = true;
}

// -------------------------------------------

void GuiVertex::print() {
  printf("x: %3.2f, y: %3.2f, r: %3.2f, g: %3.2f, b: %3.2f a: %3.2f\n", pos[0], pos[1], color[0], color[1], color[2], color[3]);
}

// Utils
// -------------------------------------------

GLuint gui_create_shader(GLenum type, const char* src) {
  GLuint s = glCreateShader(type);
  glShaderSource(s, 1, &src,  NULL);
  glCompileShader(s);
  gui_print_shader_compile_info(s);
  return s;
}

extern GLuint gui_create_program(GLuint vert, GLuint frag, int natts, const char** atts) {
  GLuint prog = glCreateProgram();
  glAttachShader(prog, vert);
  glAttachShader(prog, frag);

  for(int i = 0; i < natts; ++i) {
    glBindAttribLocation(prog, i, atts[i]);
  }

  glLinkProgram(prog);

  gui_print_program_link_info(prog);

  return prog;
}

extern void gui_print_program_link_info(GLuint program) {
  GLint status = 0;
  GLint count = 0;
  GLchar* error = NULL;
  GLsizei nchars = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if(!status) {
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &count);
    if(count > 0) {
      error = new GLchar[count];
      glGetProgramInfoLog(program, 2048, &nchars, error);
      printf("------\n");
      printf("%s\n", error);
      printf("------\n");
      delete[] error;
      error = NULL;
      assert(0);
    }
  }
}

extern void gui_print_shader_compile_info(GLuint shader) {
  GLint status = 0;
  GLint count = 0;
  GLchar* error = NULL;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if(!status) {
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &count);
    if(count > 0) {
      error = new GLchar[count];
      glGetShaderInfoLog(shader, count, NULL, error);
      printf("------\n");
      printf("%s\n", error);
      printf("------\n");
      delete[] error;
      error = NULL;
      assert(0);
    }
  }
}

void gui_ortho(float l, float r, float b, float t, float n, float f, float* dest) {
  dest[0] = (2.0f / (r - l));
  dest[1] = 0.0f;
  dest[2] = 0.0f;
  dest[3] = 0.0f;

  dest[4] = 0.0f;
  dest[5] = (2.0f / (t - b));
  dest[6] = 0.0f;
  dest[7] = 0.0f;
  
  dest[8] = 0.0f; 
  dest[9] = 0.0f;
  dest[10] = (-2.0f / (f - n));
  dest[11] = 0.0f;

  dest[12] = - ((r + l) / (r - l));
  dest[13] = - ((t + b) / (t - b));
  dest[14] = - ((f + n) / (f - n));
  dest[15] = 1.0f;
}

#endif
