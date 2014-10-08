// ------------------------------------------------------------------------------
// H E A D E R
// ------------------------------------------------------------------------------

#ifndef REMOXLY_RENDER_GL_H
#define REMOXLY_RENDER_GL_H

// -------------------------------------------

#define RENDER_GL2 1                         /* Use GL2, legacy support */
#define RENDER_GL3 2                         /* Use GL3+ */

#if !defined(RENDER_GL)
#  define RENDER_GL RENDER_GL3
#endif

#if RENDER_GL == RENDER_GL3
#  define BITMAP_FONT_GL BITMAP_FONT_GL3
#elif RENDER_GL == RENDER_GL2
#  define BITMAP_FONT_GL BITMAP_FONT_GL2
#endif

#if defined(__APPLE__) && RENDER_GL == RENDER_GL2
#  define glGenVertexArrays glGenVertexArraysAPPLE
#  define glBindVertexArray glBindVertexArrayAPPLE
#endif

// -------------------------------------------

#define TEXT_INPUT_IMPLEMENTATION
#define BITMAP_FONT_IMPLEMENTATION
#include <gui/bitmapfont/BitmapFont.h>
#include <gui/textinput/TextInput.h>

/* @todo - pick a font in RenderGL header */
#include <gui/fonts/FreePixel.h>
#include <gui/fonts/FontAwesome.h>
#include <gui/fonts/SourceCode.h>
#include <gui/fonts/DejaVu.h>
#include <gui/fonts/Arial.h>

#include <gui/Render.h>
#include <assert.h>
#include <algorithm>
#include <map> /* @todo cleanup, experimental for layers */

/* Experimental for rounded rectangles. */
/* @todo - begin - when we don't need rounded rectangles remove this. */
#define PI 3.14159265359
#define TWO_PI 6.28318530718
#define HALF_PI 1.57079632679
/* @todo - end - when we don't need rounded rectangles remove this. */

// -------------------------------------------

#if RENDER_GL == RENDER_GL2
static const char* GUI_RENDER_VS = ""
  "#version 110\n"
  "uniform mat4 u_pm;"
  "attribute vec4 a_pos;"
  "attribute vec4 a_color;"
  "varying vec4 v_color;"
  "void main() {"
  "  gl_Position = u_pm * a_pos;"
  "  v_color = a_color;"
  "}"
  "";

static const char* GUI_RENDER_FS = ""
  "#version 110\n"
  "varying vec4 v_color;"
  "void main() {"
  "  gl_FragColor = v_color;"
  "}"
  "";

// Position + TexCoord 
// -------------------------------------------
static const char* GUI_RENDER_PT_VS = ""
  "#version 110\n"
  "uniform mat4 u_pm;"
  "attribute vec4 a_pos;"
  "attribute vec2 a_tex;"
  "varying vec2 v_tex;"
  ""
  "void main() {"
  "  gl_Position = u_pm * a_pos;"
  "  v_tex = a_tex;"
  "}"
  "";

static const char* GUI_RENDER_PT_FS = ""
  "#version 110\n"
  "uniform sampler2D u_tex;"
  "varying vec2 v_tex;"
  ""
  "void main() {"
  "  gl_FragColor = texture2D(u_tex, v_tex);"
  "}"
  "";

static const char* GUI_RENDER_PT_RECT_FS = ""
  "#version 110\n"
  "uniform sampler2DRect u_tex;"
  "varying vec2 v_tex;"
  ""
  "void main() {"
  "  gl_FragColor = texture2DRect(u_tex, v_tex);"
  "}"
  "";
#endif

// -------------------------------------------
#if RENDER_GL == RENDER_GL3
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

// Position + TexCoord 
// -------------------------------------------
static const char* GUI_RENDER_PT_VS = ""
  "#version 150\n"
  "uniform mat4 u_pm;"
  "in vec4 a_pos;"
  "in vec2 a_tex;"
  "out vec2 v_tex;"
  ""
  "void main() {"
  "  gl_Position = u_pm * a_pos;"
  "  v_tex = a_tex;"
  "}"
  "";

static const char* GUI_RENDER_PT_FS = ""
  "#version 150\n"
  "uniform sampler2D u_tex;"
  "in vec2 v_tex;"
  "out vec4 fragcolor;"
  ""
  "void main() {"
  "  fragcolor = vec4(1.0, 0.0, 0.0, 1.0);"
  "  fragcolor = texture(u_tex, v_tex);"
  "}"
  "";

static const char* GUI_RENDER_PT_RECT_FS = ""
  "#version 150\n"
  "uniform sampler2DRect u_tex;"
  "in vec2 v_tex;"
  "out vec4 fragcolor;"
  ""
  "void main() {"
  "  fragcolor = texture(u_tex, v_tex);"
  "}"
  "";
#endif

// -------------------------------------------

namespace rx { 

  // position + colors
  struct GuiVertexPC {

  public:
    GuiVertexPC();
    GuiVertexPC(float x, float y, float r, float g, float b, float a);
    GuiVertexPC(float x, float y, float* col);
    void setPos(float x, float y);
    void setColor(float r, float g, float b, float a);
    const float* ptr() { return &pos[0]; } 
    void print();

  public:
    float pos[2];
    float color[4];
  };

  // -------------------------------------------

  // position + texcoords
  struct GuiVertexPT {

  public:
    GuiVertexPT();
    GuiVertexPT(float x, float y, float u, float v);
    void setPos(float x, float y);
    void setTexCoord(float u, float v);
    const float* ptr() { return &pos[0]; } 

  public:
    float pos[2];
    float tex[2];
  };

  // -------------------------------------------

  class TextureInfoGL : public TextureInfo {
  public:
    TextureInfoGL(GLenum type, int id);
    int getWidth();
    int getHeight();

  public:
    GLuint id;
    GLenum type;
    int tex_w;
    int tex_h;
  };

  // -------------------------------------------

  struct TextureDrawInfo {

    TextureDrawInfo();

    int offset;
    int count;
    TextureInfoGL* info;
  };

  struct TextureDrawInfoSorter {
    bool operator()(const TextureDrawInfo& a, const TextureDrawInfo& b) { return a.info->type > b.info->type; } 
  };

  // -------------------------------------------

  GLuint gui_create_shader(GLenum type, const char* src);
  GLuint gui_create_program(GLuint vert, GLuint frag, int natts = 0, const char** atts = NULL);
  void gui_print_program_link_info(GLuint prog);
  void gui_print_shader_compile_info(GLuint shader);
  void gui_ortho(float l, float r, float b, float t, float n, float f, float* dest);

  // -------------------------------------------

  class RenderLayer {
  public:
    RenderLayer();

  public:
    std::vector<GLint> bg_offsets;
    std::vector<GLsizei> bg_counts;
    std::vector<GLint> fg_offsets;
    std::vector<GLsizei> fg_counts;
    DejaVu text_font;
    DejaVu number_font;
    FontAwesome icon_font;
    DejaVu text_input_font;
    DejaVu number_input_font;
    TextInput text_input;
    TextInput number_input;
  };

  class RenderGL : public Render {

  public:
    RenderGL(int gl = RENDER_GL3);

    void update();
    void draw();
    void resize(int w, int h);

    void getWindowSize(int& ww, int& wh);
    void beginScissor();
    void scissor(int sx, int sy, int sw, int sh);
    void endScissor();

    void clear();                                                                             /* removes all vertices, and offset data */
    void writeText(float x, float y, std::string text, float* color);                         /* write some text */
    void writeNumber(float x, float y, std::string number, float* color);                     /* write a number value; we right align it */
    void writeIcon(float x, float y, unsigned int icon, float* color);                        /* write an icon. see Types.h for the available icons */
    void addRectangle(float x, float y, float w, float h, float* color, bool filled = true, float shadetop = -0.15, float shadebot = 0.15f);  /* draw a rectangle at x/y with w/h and given color; must have 4 elements */
    void addRectangle(float x, float y, float w, float h, TextureInfo* texinfo);              /* draw a rectangle at x/y with w/h for the given texture */

    /* experimental */
    void addRoundedRectangle(float x, float y, float w, float h, float radius, float* color, bool filled = true, int corners = GUI_CORNER_ALL); 
    void setLayer(int layer); 
    /* end experimental */

    void enableTextInput(float x, float y, float maxw, std::string value, float* color); 
    void enableNumberInput(float x, float y, float maxw, std::string value, float* color);
    void disableNumberInput();
    void disableTextInput();
    void getNumberInputValue(std::string& result);
    void getTextInputValue(std::string& result);
    bool getIconSize(unsigned int id, int& w, int& h);

    void onCharPress(unsigned int key); /* gets called when a key is pressed (called by an editable widget) */
    void onKeyPress(int key, int mods); /* gets called for special keys (called by an editable widget) */

  private: 
    void updatePositionColorBuffers();
    void updatePositionTexCoordBuffers();

  public:

    /* opengl */
    int gl_version;                                              /* passed into the constructor; either RENDER_GL2 or RENDER_GL3 */
    GLint viewport[4];                                           /* contains the viewport size; is e.g. used in getWindowSize() */
    static bool is_initialized;                                  /* static member, is set to true when the shaders/prog has been created */

    /* GuiVertexPC: lines + background */
    GLuint vbo_pc;                                               /* the vbo that keeps the vertices for the position + colors */
    GLuint vao_pc;                                               /* vao to keep state of the vbo for position + colors */
    static GLuint prog_pc;                                       /* the shader program that we use to render everything */
    static GLuint vert_pc;                                       /* the vertex shader for the gui */
    static GLuint frag_pc;                                       /* the fragment shader for the gui */

    /* GuiVertexPT: textures */
    GLuint vbo_pt;                                               /* vbo for the position + texcoord buffers */
    GLuint vao_pt;                                               /* vao for the position + textcoord buffers */
    static GLuint prog_pt;                                       /* shader program that renders GuiVertexPT */
    static GLuint prog_pt_rect;
    static GLuint vert_pt;                                       /* vertex shader that renders GuiVertexPT */
    static GLuint frag_pt;                                       /* fragment shader that renders GuiVertexPT */
    static GLuint frag_pt_rect;

    /* GuiVertexPC buffer info */
    bool needs_update_pc;                                        /* set to true whenever we need to update the vbo for the position + color type*/
    size_t bytes_allocated_pc;                                   /* how many bytes we've allocated in the vbo for the position + color type */
    std::vector<GuiVertexPC> vertices_pc;                        /* vertices for that make up the gui (for color + position) */
    std::vector<GLint> bg_offsets;                               /* offsets of the different background elements */
    std::vector<GLsizei> bg_counts;                              /* vertex counts for the background elements */
    std::vector<GLint> fg_offsets;                               /* offsets of the foreground elements */
    std::vector<GLsizei> fg_counts;                              /* vetex counts for the foreground elements */

    /* GuiVertexPT buffer info */
    bool needs_update_pt;                                        /* is set to true whenever we need t update the pos/tex vertices */
    size_t bytes_allocated_pt;                                   /* the number of bytes allocated in the GuiVertexPT buffer */
    std::vector<GuiVertexPT> vertices_pt;                        /* the vertices that we use to draw a texture */
    std::vector<TextureDrawInfo> texture_draws;                  /* keeps information about the textures we need to draw */

    /* @todo cleanup */
    //    int layer_num; /* @todo cleanup, current layer that is being drawn to. */
    std::map<int, RenderLayer*> layers; /* @todo cleanup, this is experimental, trying to add a depth sorting. */
    RenderLayer* layer;
    /* ------------ */

    /* fonts */
    DejaVu text_font;
    //SourceCode number_font;
    DejaVu number_font;
    FontAwesome icon_font;
    //Arial text_input_font;                                       /* we need to use another font object for the text input because the text input clears all vertices, so it cannot share the text_font. */
    //SourceCode number_input_font;                                /* we need to use another font object for the number font because the number font clears all vertices, so it cannot share the number_font. */
    DejaVu text_input_font;
    DejaVu number_input_font;
    TextInput text_input;
    TextInput number_input;
  };

 
  /* interpolates the given colors `a` and `b` using the percentage set by `fac` */
  inline void gl_interpolate_colors(const float a[4], const float b[4], const float fac, float out[4]) {
    float inv = 1.0 - fac;
    out[0] = fac * a[0] + inv * b[0];
    out[1] = fac * a[1] + inv * b[1];
    out[2] = fac * a[2] + inv * b[2];
    out[3] = fac * a[3] + inv * b[3];
  }

  inline void gl_interpolate_colors_with_range(const float a[4], const float b[4], float minval, float maxval, float val, float out[4]) {
    float p = (maxval - val) / (maxval - minval);
    gl_interpolate_colors(a, b, p, out);
  }

  inline void gl_shade_colors(float colin[4], float shadea, float shadeb, float outa[4], float outb[4]) {
    outa[0] = std::min<float>(1.0f, colin[0] + shadea);
    outa[1] = std::min<float>(1.0f, colin[1] + shadea);
    outa[2] = std::min<float>(1.0f, colin[2] + shadea);
    outa[3] = colin[3];
    outb[0] = std::max<float>(0.0f, colin[0] + shadeb);
    outb[1] = std::max<float>(0.0f, colin[1] + shadeb);
    outb[2] = std::max<float>(0.0f, colin[2] + shadeb);
    outb[3] = colin[3];
  }

  inline void gl_add_shaded_vertex_pc(float x, float y, float miny, float maxy, 
                                      const float coltop[4], const float colbot[4], float intcol[4],
                                      std::vector<GuiVertexPC>& result)
  {
    gl_interpolate_colors_with_range(coltop, colbot, miny, maxy, y, intcol);
    result.push_back(GuiVertexPC(x, y, intcol));
  }

  /* @todo - this is experimental */
  inline void RenderGL::setLayer(int l) {

    std::map<int, RenderLayer*>::iterator it = layers.find(l);
    if (it == layers.end()) {
      layer = new RenderLayer();
      if (NULL == layer) {
        printf("Cannot allocate new RenderLayer.\n");
        return;
      }
      layers[l] = layer;
    }
    else {
      layer = it->second;
    }
    assert(layer);
  }

} // namespace rx

#endif

// ------------------------------------------------------------------------------
// I M P L E M E N T A T I O N
// ------------------------------------------------------------------------------

#if defined(REMOXLY_IMPLEMENTATION)

namespace rx { 


  // -------------------------------------------

  RenderLayer::RenderLayer()
    :text_input(0.0f, 0.0f, 0.0f, text_input_font)
    ,number_input(0.0f, 0.0f, 0.0f, number_input_font)
  {
    /* @todo we probably can remove the text_input font as we're using the same font for text and numbers now. */
    /* @todo cleanup this init... */
    if(!text_font.setup()) {
      printf("Error: cannot setup text font.\n");
    }

    if(!icon_font.setup()) {
      printf("Error: cannot setup icon font.\n");
    }

    if(!number_font.setup()) {
      printf("Error: cannot setup number font.\n");
    }

    if(!text_input_font.setup()) {
      printf("Error: cannot setup the text input font.\n");
    }

    if(!number_input_font.setup()) {
      printf("Error: cannot setup the number input font.\n");
    }

    number_input.align = BITMAP_FONT_ALIGN_RIGHT;
    text_input.align = BITMAP_FONT_ALIGN_LEFT;
    
  }

  // -------------------------------------------

  GLuint RenderGL::prog_pc = 0;
  GLuint RenderGL::vert_pc = 0;
  GLuint RenderGL::frag_pc = 0;
  GLuint RenderGL::prog_pt = 0;
  GLuint RenderGL::vert_pt = 0;
  GLuint RenderGL::frag_pt = 0;
  GLuint RenderGL::frag_pt_rect = 0;
  GLuint RenderGL::prog_pt_rect = 0;
  bool RenderGL::is_initialized = false;

  // -------------------------------------------

  RenderGL::RenderGL(int gl) 
    :gl_version(gl)
    ,vbo_pc(0)
    ,vao_pc(0)
    ,bytes_allocated_pc(0)
    ,bytes_allocated_pt(0)
    ,needs_update_pc(false)
    ,needs_update_pt(false)
    ,layer(NULL) 
    ,text_input(0.0f, 0.0f, 0.0f, text_input_font)
    ,number_input(0.0f, 0.0f, 0.0f, number_input_font)
  {
    
    // ortho projection 
    float pm[16];
    viewport[0] = viewport[1] = viewport[2] = viewport[3] = 0;
    glGetIntegerv(GL_VIEWPORT, viewport);
    gui_ortho(0.0f, viewport[2], viewport[3], 0.0f, 0.0f, 100.0f, pm);

    if(!is_initialized) {

      // shader for pos + color
      const char* atts_pc[] = { "a_pos", "a_color" };
      vert_pc = gui_create_shader(GL_VERTEX_SHADER, GUI_RENDER_VS);
      frag_pc = gui_create_shader(GL_FRAGMENT_SHADER, GUI_RENDER_FS);
      prog_pc = gui_create_program(vert_pc, frag_pc, 2, atts_pc);

      // shader for pos + texcoord
      const char* atts_pt[] = { "a_pos", "a_tex" } ;
      vert_pt = gui_create_shader(GL_VERTEX_SHADER, GUI_RENDER_PT_VS);
      frag_pt = gui_create_shader(GL_FRAGMENT_SHADER, GUI_RENDER_PT_FS);
      prog_pt = gui_create_program(vert_pt, frag_pt, 2, atts_pt);

      // shader for pos + texcoord
      const char* atts_pt_rect[] = { "a_pos", "a_tex" } ;
      frag_pt_rect = gui_create_shader(GL_FRAGMENT_SHADER, GUI_RENDER_PT_RECT_FS);
      prog_pt_rect = gui_create_program(vert_pt, frag_pt_rect, 2, atts_pt_rect);

      glUseProgram(prog_pc);
      glUniformMatrix4fv(glGetUniformLocation(prog_pc, "u_pm"), 1, GL_FALSE, pm);

      glUseProgram(prog_pt);
      glUniformMatrix4fv(glGetUniformLocation(prog_pt, "u_pm"), 1, GL_FALSE, pm);
      glUniform1i(glGetUniformLocation(prog_pt, "u_tex"), 0);

      glUseProgram(prog_pt_rect);
      glUniformMatrix4fv(glGetUniformLocation(prog_pt_rect, "u_pm"), 1, GL_FALSE, pm);
      glUniform1i(glGetUniformLocation(prog_pt_rect, "u_tex"), 0);

      is_initialized = true;
    }

    // pos + color vao,vbo
    glGenVertexArrays(1, &vao_pc);
    glBindVertexArray(vao_pc);

    glGenBuffers(1, &vbo_pc);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pc);

    glEnableVertexAttribArray(0); // pos
    glEnableVertexAttribArray(1); // color

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GuiVertexPC), (GLvoid*)0); // pos
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GuiVertexPC), (GLvoid*)8); // col

    // pos + texcoord vao,vbo
    glGenVertexArrays(1, &vao_pt);
    glBindVertexArray(vao_pt);

    glGenBuffers(1, &vbo_pt);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_pt);

    glEnableVertexAttribArray(0); // pos
    glEnableVertexAttribArray(1); // texcoord

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GuiVertexPT), (GLvoid*)0); // pos
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GuiVertexPT), (GLvoid*)8); // texcoord

    if(!text_font.setup()) {
      printf("Error: cannot setup text font.\n");
      return;
    }

    if(!icon_font.setup()) {
      printf("Error: cannot setup icon font.\n");
      return;
    }

    if(!number_font.setup()) {
      printf("Error: cannot setup number font.\n");
      return;
    }

    if(!text_input_font.setup()) {
      printf("Error: cannot setup the text input font.\n");
      return;
    }

    if(!number_input_font.setup()) {
      printf("Error: cannot setup the number input font.\n");
      return;
    }

    number_input.align = BITMAP_FONT_ALIGN_RIGHT;
    text_input.align = BITMAP_FONT_ALIGN_LEFT;

    /* @todo cleanup - here we create the bottom layer */
    setLayer(0);
    /* @todo end cleanup */
  }

  void RenderGL::getWindowSize(int& ww, int& wh) {
    ww = viewport[2];
    wh = viewport[3];
  }

  void RenderGL::update() {
  
    updatePositionColorBuffers();
    updatePositionTexCoordBuffers();
  }

  void RenderGL::updatePositionColorBuffers() {

    if(!vertices_pc.size()) {
      return;
    }
  
    if(!needs_update_pc) {
      return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo_pc);

    size_t needed = vertices_pc.size() * sizeof(GuiVertexPC);

    if(needed > bytes_allocated_pc) {
      glBufferData(GL_ARRAY_BUFFER, needed, vertices_pc[0].ptr(), GL_STREAM_DRAW);
      bytes_allocated_pc = needed;
    }
    else {
      glBufferSubData(GL_ARRAY_BUFFER, 0, needed, vertices_pc[0].ptr());
    }

    needs_update_pc = false;
  }

  void RenderGL::updatePositionTexCoordBuffers() {

    if(!vertices_pt.size()) {
      return;
    }

    if(!needs_update_pt) {
      return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo_pt);

    size_t needed = vertices_pt.size() * sizeof(GuiVertexPT);

    if(needed > bytes_allocated_pt) {
      glBufferData(GL_ARRAY_BUFFER, needed, vertices_pt[0].ptr(), GL_STREAM_DRAW);
      bytes_allocated_pt = needed;
    }
    else {
      glBufferSubData(GL_ARRAY_BUFFER, 0, needed, vertices_pt[0].ptr());
    }

    needs_update_pt = false;
  }

  void RenderGL::draw() {

    /* RENDER LAYER IMPLEMENTATION */
    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (vertices_pc.size()) {

      RenderLayer* lyr = NULL;
      std::map<int, RenderLayer*>::iterator it = layers.begin();

      while (it != layers.end()) {

        glUseProgram(prog_pc);
        glBindVertexArray(vao_pc);

        lyr = it->second;

        if(lyr->bg_counts.size()) {
          glMultiDrawArrays(GL_TRIANGLES, &lyr->bg_offsets[0], &lyr->bg_counts[0], lyr->bg_counts.size());
        }

        if(lyr->fg_counts.size()) {
          glMultiDrawArrays(GL_LINE_STRIP, &lyr->fg_offsets[0], &lyr->fg_counts[0], lyr->fg_counts.size());
        } 

        lyr->icon_font.draw();
        lyr->text_font.draw();
        lyr->number_font.draw();
        lyr->text_input.draw();
        lyr->number_input.draw();

        ++it;
      }
    }

    glDisable(GL_BLEND);


    /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
#if 0
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if(vertices_pc.size()) {

      glUseProgram(prog_pc);
      glBindVertexArray(vao_pc);

      if(bg_counts.size()) {
        glMultiDrawArrays(GL_TRIANGLES, &bg_offsets[0], &bg_counts[0], bg_counts.size());
      }

      if(fg_counts.size()) {
        glMultiDrawArrays(GL_LINE_STRIP, &fg_offsets[0], &fg_counts[0], fg_counts.size());
      }
    }

    if(texture_draws.size()) {

      glBindVertexArray(vao_pt);
      glActiveTexture(GL_TEXTURE0);

      GLenum type = 0;

      for(std::vector<TextureDrawInfo>::iterator it = texture_draws.begin(); it != texture_draws.end(); ++it) {

        TextureDrawInfo& tex = *it;

        if(type != tex.info->type) {
          if(tex.info->type == GL_TEXTURE_2D) {
            glUseProgram(prog_pt);
          }
#if RENDER_GL == RENDER_GL2
          else if(tex.info->type == GL_TEXTURE_RECTANGLE_ARB) {
            glUseProgram(prog_pt_rect);
          }
#elif RENDER_GL == RENDER_GL3
          else if(tex.info->type == GL_TEXTURE_RECTANGLE) {
            glUseProgram(prog_pt_rect);
          }
#endif
          else {
            printf("Error: unsupported texture target.\n");
          }
        }

        glBindTexture(tex.info->type, tex.info->id);
        glDrawArrays(GL_TRIANGLES, tex.offset, tex.count);
      }
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    icon_font.draw();
    text_font.draw();
    number_font.draw();
    text_input.draw();
    number_input.draw();

    glDisable(GL_BLEND);
#endif
  }

  void RenderGL::resize(int w, int h) {

    float pm[16];
    gui_ortho(0.0f, w, h, 0.0f, 0.0f, 100.0f, pm);

    glUseProgram(prog_pc);
    glUniformMatrix4fv(glGetUniformLocation(prog_pc, "u_pm"), 1, GL_FALSE, pm);

    glUseProgram(prog_pt);
    glUniformMatrix4fv(glGetUniformLocation(prog_pt, "u_pm"), 1, GL_FALSE, pm);

    glUseProgram(prog_pt_rect);
    glUniformMatrix4fv(glGetUniformLocation(prog_pt_rect, "u_pm"), 1, GL_FALSE, pm);
  
    icon_font.resize(w, h);
    text_font.resize(w, h);
    number_font.resize(w, h);
    text_input_font.resize(w, h);
    number_input_font.resize(w, h);

    viewport[2] = w;
    viewport[3] = h;
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

    vertices_pc.clear();
    bg_offsets.clear();
    bg_counts.clear();
    fg_offsets.clear();
    fg_counts.clear();

    vertices_pt.clear();
    texture_draws.clear();
  
    icon_font.clear();
    text_font.clear();
    number_font.clear();
    text_input_font.clear();
    number_input_font.clear();

    /* @todo - begin clean up, experimental implementing layers. */
    std::map<int, RenderLayer*>::iterator it = layers.begin();
    while (it != layers.end()) {

      RenderLayer* l = it->second;
      l->bg_offsets.clear();
      l->bg_counts.clear();
      l->fg_offsets.clear();
      l->fg_counts.clear();
      l->text_font.clear();
      l->number_font.clear();
      l->icon_font.clear();
      l->text_input_font.clear();
      l->number_input_font.clear();
      ++it;
    }

    /* @todo - end experimental */
  }

  void RenderGL::onCharPress(unsigned int key) {

    if(text_input.mode != TI_MODE_DISABLED) {
      text_input.onCharPress(key); /* @todo cleanup, old imp. */
      layer->text_input.onCharPress(key); 
    }

    if(number_input.mode != TI_MODE_DISABLED) {
      number_input.onCharPress(key); /* @todo cleanup, old implementation. */
      layer->number_input.onCharPress(key);
    }
  }

  void RenderGL::onKeyPress(int key, int mods) {

    if(text_input.mode != TI_MODE_DISABLED) {
      text_input.onKeyPress(key, mods); /* @todo cleanup, old implemntation. */
      layer->text_input.onKeyPress(key, mods);
    }

    if(number_input.mode != TI_MODE_DISABLED) {
      number_input.onKeyPress(key, mods); /* @todo cleanup, old implementation. */
      layer->number_input.onKeyPress(key, mods);
    }
  }

  void RenderGL::writeText(float x, float y, std::string text, float* color) {

    text_font.setColor(color[0], color[1], color[2], color[3]);
    text_font.write(x, y, text, BITMAP_FONT_ALIGN_LEFT);

    /* @todo cleanup -> implementing layers. */
    layer->text_font.setColor(color[0], color[1], color[2], color[3]);
    layer->text_font.write(x, y, text, BITMAP_FONT_ALIGN_LEFT);
  }

  void RenderGL::writeNumber(float x, float y, std::string number, float* color) {

    number_font.setColor(color[0], color[1], color[2], color[3]);
    number_font.write(x, y, number, BITMAP_FONT_ALIGN_RIGHT);

    /* @todo cleanup -> implementing layers. */
    layer->number_font.setColor(color[0], color[1], color[2], color[3]);
    layer->number_font.write(x, y, number, BITMAP_FONT_ALIGN_RIGHT);
  }

  void RenderGL::writeIcon(float x, float y, unsigned int icon, float* color) {
  
    icon_font.setColor(color[0], color[1], color[2], color[3]);
    icon_font.write(x, y, icon);

    /* @todo cleanup -> implementing layers. */
    layer->icon_font.setColor(color[0], color[1], color[2], color[3]);
    layer->icon_font.write(x, y, icon);
  }

  void RenderGL::enableTextInput(float x, float y, float maxw, std::string value, float* color) {

    /* @todo - old, cleanup. */
    text_input.x = x;
    text_input.y = y;
    text_input.w = maxw;

    text_input.clear(); // @todo this might be a duplicate call, when a widget makes its state editable RenderGL::clear() will be called too.
    text_input.enable();
    text_input.setValue(value);
    text_input.select();

    /* @todo - begin - cleanup, this is the new imp. */
    layer->text_input.x = x;
    layer->text_input.y = y;
    layer->text_input.w = maxw;

    layer->text_input.clear(); // @todo this might be a duplicate call, when a widget makes its state editable RenderGL::clear() will be called too.
    layer->text_input.enable();
    layer->text_input.setValue(value);
    layer->text_input.select();
  }

  void RenderGL::disableTextInput() {
    /* @todo cleanup , this is old. */
    text_input.clear();
    text_input.disable();

    layer->text_input.clear();
    layer->text_input.disable();
  }

  void RenderGL::getTextInputValue(std::string& result) {
    // result = text_input.getValue(); /* @todo cleanup */
    result = layer->text_input.getValue(); 
  }

  void RenderGL::disableNumberInput() {
    number_input.clear();
    number_input.disable();

    /* @todo - begin - this is the new implementation using layers. */
    layer->number_input.clear();
    layer->number_input.disable();
    /* @todo - end - */
  }

  void RenderGL::getNumberInputValue(std::string& result) {
    /* @todo - begin - this is the new implementation using layers. */
    result = layer->number_input.getValue();
    /* @todo - end - */
  
    //result = number_input.getValue(); // @todo commented because of layer implementation. 
  }

  void RenderGL::enableNumberInput(float x, float y, float maxw, std::string value, float* color) {
    number_input.x = x;
    number_input.y = y;
    number_input.w = maxw;
  
    number_input.clear();
    number_input.enable();
    number_input.setValue(value);
    number_input.select();

    /* @todo - begin - this is the new implementation using layers. */
    layer->number_input.x = x;
    layer->number_input.y = y;
    layer->number_input.w = maxw;

    layer->number_input.clear();
    layer->number_input.enable();
    layer->number_input.setValue(value);
    layer->number_input.select();
    /* @todo - end - */
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

  void RenderGL::addRectangle(float x, float y, float w, float h, float* color, bool filled, float shadetop, float shadebot) {

    float coltop[4]; /* color for the top */
    float colbot[4]; /* color for the bottom */
    float min_y = y;
    float max_y = y + h;
    float cola[4];
    float colb[4];

    gl_shade_colors(color, shadetop, shadebot, coltop, colbot);
    gl_interpolate_colors_with_range(coltop, colbot, min_y, max_y, min_y, cola);
    gl_interpolate_colors_with_range(coltop, colbot, min_y, max_y, max_y, colb);

    GuiVertexPC a(x, y + h, colb);     // bottom left
    GuiVertexPC b(x + w, y + h, colb); // bottom right
    GuiVertexPC c(x + w, y, cola);     // top right 
    GuiVertexPC d(x, y, cola);         // top left
  
    if(filled) { 
      bg_offsets.push_back(vertices_pc.size());
      layer->bg_offsets.push_back(vertices_pc.size()); /* @todo cleanup */

      vertices_pc.push_back(a);
      vertices_pc.push_back(b);
      vertices_pc.push_back(c);

      vertices_pc.push_back(a);
      vertices_pc.push_back(c);
      vertices_pc.push_back(d);

      bg_counts.push_back(vertices_pc.size() - bg_offsets.back());
      layer->bg_counts.push_back(vertices_pc.size() - bg_offsets.back()); /* @todo cleanup */
    }
    else {
      fg_offsets.push_back(vertices_pc.size());
      layer->fg_offsets.push_back(vertices_pc.size()); /* @todo cleanup */
      vertices_pc.push_back(a);
      vertices_pc.push_back(b);
      vertices_pc.push_back(c);
      vertices_pc.push_back(d);
      vertices_pc.push_back(a);
      fg_counts.push_back(vertices_pc.size() - fg_offsets.back());
      layer->fg_counts.push_back(vertices_pc.size() - fg_offsets.back()); /* @todo cleanup */
    }

    needs_update_pc = true;
  }

  void RenderGL::addRectangle(float x, float y, float w, float h, TextureInfo* tex) {

    TextureInfoGL* tex_gl = static_cast<TextureInfoGL*>(tex);
    float tex_w = 1.0f;
    float tex_h = 1.0f;

#if RENDER_GL == RENDER_GL2
    if(tex_gl->type == GL_TEXTURE_RECTANGLE_ARB) {
      tex_w = tex_gl->tex_w;
      tex_h = tex_gl->tex_h;
    }
#elif RENDER_GL == RENDER_GL3
    if(tex_gl->type == GL_TEXTURE_RECTANGLE) {
      tex_w = tex_gl->tex_w;
      tex_h = tex_gl->tex_h;
    }
#endif
  
    GuiVertexPT a(x, y + h, 0.0f, tex_h);      // bottom left
    GuiVertexPT b(x + w, y + h, tex_w, tex_h); // bottom right
    GuiVertexPT c(x + w, y, tex_w, 0.0f);      // top right
    GuiVertexPT d(x, y, 0.0f, 0.0f);           // top left;

    TextureDrawInfo draw_info;
    draw_info.offset = vertices_pt.size();
    draw_info.info = tex_gl;

    vertices_pt.push_back(a);
    vertices_pt.push_back(b);
    vertices_pt.push_back(c);
    vertices_pt.push_back(a);
    vertices_pt.push_back(c);
    vertices_pt.push_back(d);

    draw_info.count = vertices_pt.size() - draw_info.offset;

    texture_draws.push_back(draw_info);

    std::sort(texture_draws.begin(), texture_draws.end(), TextureDrawInfoSorter());

    needs_update_pt = true;
  }

  void RenderGL::addRoundedRectangle(float x, float y, float w, float h, float radius, float* color, bool filled, int corners) {

#if 1
    int resolution = 7;
    float angle = 0.0;
    float rx = 0.0f;
    float ry = 0.0f;
    float min_y = y;
    float max_y = y + h;
    float shadetop = 0.10f;
    float shadebot = -0.10f;
    float coltop[4]; /* color for the top */
    float colbot[4]; /* color for the bottom */
    float intcol[4]; /* interpolated color */
    float ca = 0.0f;
    float sa = 0.0f;

    gl_shade_colors(color, shadetop, shadebot, coltop, colbot);

    /* Calculate the sin/cos values. */
    std::vector<float> points;
    for (int i = 0; i <= resolution; ++i) {
      ca = radius * cos(angle);
      sa = radius * sin(angle);
      points.push_back(ca);
      points.push_back(sa);
      angle += (HALF_PI / resolution);
    }

    bg_offsets.push_back(vertices_pc.size());
    layer->bg_offsets.push_back(vertices_pc.size()); /* @todo layer */

    /* bottom right corner. */
    rx = x + w - radius;
    ry = y + h - radius;
    if (corners & GUI_CORNER_BOTTOM_RIGHT) {
      for (size_t i = 0; i < (points.size()/2)-1; ++i) {
        gl_add_shaded_vertex_pc(rx, ry, min_y, max_y, coltop, colbot, intcol, vertices_pc);
        gl_add_shaded_vertex_pc(rx + points[i * 2 + 0], ry + points[i * 2 + 1], min_y, max_y, coltop, colbot, intcol, vertices_pc);
        gl_add_shaded_vertex_pc(rx + points[(i+1) * 2 + 0], ry + points[(i+1) * 2 + 1],  min_y, max_y, coltop, colbot, intcol, vertices_pc);
      }
    }
    else {
      gl_add_shaded_vertex_pc(rx, ry + radius, min_y, max_y, coltop, colbot, intcol, vertices_pc);
      gl_add_shaded_vertex_pc(rx + radius, ry + radius, min_y, max_y, coltop, colbot, intcol, vertices_pc);
      gl_add_shaded_vertex_pc(rx + radius, ry, min_y, max_y, coltop, colbot, intcol, vertices_pc);

      gl_add_shaded_vertex_pc(rx, ry + radius, min_y, max_y, coltop, colbot, intcol, vertices_pc);
      gl_add_shaded_vertex_pc(rx + radius, ry , min_y, max_y, coltop, colbot, intcol, vertices_pc);
      gl_add_shaded_vertex_pc(rx, ry , min_y, max_y, coltop, colbot, intcol, vertices_pc);
    }

    /* top right corner */
    rx = x + w - radius;
    ry = y + radius;
    if (corners & GUI_CORNER_TOP_RIGHT) { 
      for (size_t i = 0; i < (points.size()/2)-1; ++i) {
        gl_add_shaded_vertex_pc(rx, ry, min_y, max_y, coltop, colbot, intcol, vertices_pc);
        gl_add_shaded_vertex_pc(rx + points[i * 2 + 0], (ry - points[i * 2 + 1]), min_y, max_y, coltop, colbot, intcol, vertices_pc);
        gl_add_shaded_vertex_pc(rx + points[(i+1) * 2 + 0], (ry - points[(i+1) * 2 + 1]),min_y, max_y, coltop, colbot, intcol, vertices_pc);
      }
    }
    else {
      gl_add_shaded_vertex_pc(rx, ry, min_y, max_y, coltop, colbot, intcol, vertices_pc);
      gl_add_shaded_vertex_pc(rx + radius, ry, min_y, max_y, coltop, colbot, intcol, vertices_pc);
      gl_add_shaded_vertex_pc(rx + radius, ry - radius, min_y, max_y, coltop, colbot, intcol, vertices_pc);

      gl_add_shaded_vertex_pc(rx, ry, min_y, max_y, coltop, colbot, intcol, vertices_pc);
      gl_add_shaded_vertex_pc(rx + radius, ry - radius, min_y, max_y, coltop, colbot, intcol, vertices_pc);
      gl_add_shaded_vertex_pc(rx, ry - radius, min_y, max_y, coltop, colbot, intcol, vertices_pc);
    }

    /* fill right. */
    gl_add_shaded_vertex_pc(rx, (y + h - radius), min_y, max_y, coltop, colbot, intcol, vertices_pc);
    gl_add_shaded_vertex_pc(rx + radius, (y + h - radius), min_y, max_y, coltop, colbot, intcol, vertices_pc);
    gl_add_shaded_vertex_pc(rx + radius, ry, min_y, max_y, coltop, colbot, intcol, vertices_pc);
    gl_add_shaded_vertex_pc(rx, (y + h - radius), min_y, max_y, coltop, colbot, intcol, vertices_pc);
    gl_add_shaded_vertex_pc(rx + radius, ry, min_y, max_y, coltop, colbot, intcol, vertices_pc);
    gl_add_shaded_vertex_pc(rx, ry, min_y, max_y, coltop, colbot, intcol, vertices_pc);

    /* top left corner */
    rx = x + radius;
    ry = y + radius;
    if (corners & GUI_CORNER_TOP_LEFT) {
      for (size_t i = 0; i < (points.size()/2)-1; ++i) {
        gl_add_shaded_vertex_pc(rx, ry, min_y, max_y, coltop, colbot, intcol, vertices_pc);
        gl_add_shaded_vertex_pc(rx - points[i * 2 + 0],  (ry - points[i * 2 + 1]), min_y, max_y, coltop, colbot, intcol, vertices_pc);
        gl_add_shaded_vertex_pc(rx - points[(i+1) * 2 + 0], (ry - points[(i+1) * 2 + 1]), min_y, max_y, coltop, colbot, intcol, vertices_pc);
      }
    }
    else {
      gl_add_shaded_vertex_pc(rx - radius, ry, min_y, max_y, coltop, colbot, intcol, vertices_pc);
      gl_add_shaded_vertex_pc(rx, ry, min_y, max_y, coltop, colbot, intcol, vertices_pc);
      gl_add_shaded_vertex_pc(rx, ry - radius, min_y, max_y, coltop, colbot, intcol, vertices_pc);

      gl_add_shaded_vertex_pc(rx - radius, ry, min_y, max_y, coltop, colbot, intcol, vertices_pc);
      gl_add_shaded_vertex_pc(rx, ry - radius, min_y, max_y, coltop, colbot, intcol, vertices_pc);
      gl_add_shaded_vertex_pc(rx - radius, ry - radius, min_y, max_y, coltop, colbot, intcol, vertices_pc);
    }

    /* fill left. */
    gl_add_shaded_vertex_pc(rx - radius, (y + h - radius), min_y, max_y, coltop, colbot, intcol, vertices_pc);
    gl_add_shaded_vertex_pc(rx, ry, min_y, max_y, coltop, colbot, intcol, vertices_pc);
    gl_add_shaded_vertex_pc(rx - radius, ry, min_y, max_y, coltop, colbot, intcol, vertices_pc);
    gl_add_shaded_vertex_pc(rx - radius, (y + h - radius), min_y, max_y, coltop, colbot, intcol, vertices_pc);
    gl_add_shaded_vertex_pc(rx + radius, (y + h - radius), min_y, max_y, coltop, colbot, intcol, vertices_pc);
    gl_add_shaded_vertex_pc(rx, ry, min_y, max_y, coltop, colbot, intcol, vertices_pc);


    /* bottom left corner */
    rx = x + radius;
    ry = y + h - radius;
    if (corners & GUI_CORNER_BOTTOM_LEFT) {
      for (size_t i = 0; i < (points.size()/2)-1; ++i) {
        gl_add_shaded_vertex_pc(rx, ry, min_y, max_y, coltop, colbot, intcol, vertices_pc);
        gl_add_shaded_vertex_pc(rx - points[i * 2 + 0], ry + points[i * 2 + 1], min_y, max_y, coltop, colbot, intcol, vertices_pc);
        gl_add_shaded_vertex_pc(rx - points[(i+1) * 2 + 0], ry + points[(i+1) * 2 + 1], min_y, max_y, coltop, colbot, intcol, vertices_pc);
      }
    }
    else {
      gl_add_shaded_vertex_pc(rx - radius, ry + radius, min_y, max_y, coltop, colbot, intcol, vertices_pc);
      gl_add_shaded_vertex_pc(rx + radius, ry + radius, min_y, max_y, coltop, colbot, intcol, vertices_pc);
      gl_add_shaded_vertex_pc(rx, ry, min_y, max_y, coltop, colbot, intcol, vertices_pc);

      gl_add_shaded_vertex_pc(rx - radius, ry + radius, min_y, max_y, coltop, colbot, intcol, vertices_pc);
      gl_add_shaded_vertex_pc(rx, ry, min_y, max_y, coltop, colbot, intcol, vertices_pc);
      gl_add_shaded_vertex_pc(rx - radius, ry, min_y, max_y, coltop, colbot, intcol, vertices_pc);
    }

    /* fill center */
    gl_add_shaded_vertex_pc(x + radius, y + h, min_y, max_y, coltop, colbot, intcol, vertices_pc);
    gl_add_shaded_vertex_pc(x + w - radius, y, min_y, max_y, coltop, colbot, intcol, vertices_pc);
    gl_add_shaded_vertex_pc(x + radius, y, min_y, max_y, coltop, colbot, intcol, vertices_pc);
    gl_add_shaded_vertex_pc(x + radius, y + h, min_y, max_y, coltop, colbot, intcol, vertices_pc);
    gl_add_shaded_vertex_pc(x + w - radius, y + h, min_y, max_y, coltop, colbot, intcol, vertices_pc);
    gl_add_shaded_vertex_pc(x + w - radius, y, min_y, max_y, coltop, colbot, intcol, vertices_pc);


#endif
    bg_counts.push_back(vertices_pc.size() - bg_offsets.back());
    layer->bg_counts.push_back(vertices_pc.size() - layer->bg_offsets.back());
    needs_update_pc = true;

#if 0
    /* @todo cleanup */
    GuiVertexPC a(x, y + h, color);     // bottom left
    GuiVertexPC b(x + w, y + h, color); // bottom right
    GuiVertexPC c(x + w, y, color);     // top right 
    GuiVertexPC d(x, y, color);         // top left

    int resolution = 7;
    float radius = 4.5;
    float angle = 0.0;
    float rx = 0.0f;
    float ry = 0.0f;
    float min_y = y;
    float max_y = y + h;
    //  float botcol[] = { 0.6, 0.6, 0.6, 1.0 };
    float botcol[] = { 0.2, 0.2, 0.2, 1.0 };
    printf("COLOR: %f, %f, %f, BOTCOL: %f, %f, %f\n", color[0], color[1], color[2], botcol[0], botcol[1], botcol[2]);

    std::vector<float> points;

    for (int i = 0; i <= resolution; ++i) {
      points.push_back(radius * cos(angle));
      points.push_back(radius * sin(angle));
      angle += (HALF_PI / resolution);
    }


    if (filled) {
      bg_offsets.push_back(vertices_pc.size());
     
      /* bottom right corner */
      rx = x + w - radius;
      ry = y + h - radius;
      bool draw_br = true;
      if (draw_br) {
        for (size_t i = 0; i < (points.size()/2)-1; ++i) {
          vertices_pc.push_back(GuiVertexPC(rx, ry, color, botcol,  min_y, max_y));
          vertices_pc.push_back(GuiVertexPC(rx + points[i * 2 + 0], ry + points[i * 2 + 1], color, botcol,  min_y, max_y));
          vertices_pc.push_back(GuiVertexPC(rx + points[(i+1) * 2 + 0], ry + points[(i+1) * 2 + 1], color, botcol,  min_y, max_y));
        }
      }
      else {
        GuiVertexPC ca(x + w - radius, y + h, color, botcol, min_y, max_y); 
        GuiVertexPC cb(x + w, y + h, color, botcol, min_y, max_y); 
        GuiVertexPC cc(x + w, y + h - radius, color, botcol, min_y, max_y); 
        GuiVertexPC cd(x + w - radius, y + h - radius, color, botcol, min_y, max_y); 
        vertices_pc.push_back(ca);
        vertices_pc.push_back(cb);
        vertices_pc.push_back(cc);
        vertices_pc.push_back(ca);
        vertices_pc.push_back(cc);
        vertices_pc.push_back(cd);
      }

      /* top right corner */
      rx = x + w - radius;
      ry = y + radius;
      for (size_t i = 0; i < (points.size()/2)-1; ++i) {
      vertices_pc.push_back(GuiVertexPC(rx, ry, color, botcol,  min_y, max_y));
      vertices_pc.push_back(GuiVertexPC(rx + points[i * 2 + 0],  (ry - points[i * 2 + 1]), color, botcol,  min_y, max_y));
      vertices_pc.push_back(GuiVertexPC(rx + points[(i+1) * 2 + 0], (ry - points[(i+1) * 2 + 1]), color, botcol,  min_y, max_y));
    }
     
      /* fill right. */
      vertices_pc.push_back(GuiVertexPC(rx, (y + h - radius), color, botcol,  min_y, max_y));
      vertices_pc.push_back(GuiVertexPC(rx + radius, (y + h - radius), color, botcol,  min_y, max_y));
      vertices_pc.push_back(GuiVertexPC(rx + radius, ry, color, botcol,  min_y, max_y));
      vertices_pc.push_back(GuiVertexPC(rx, (y + h - radius), color, botcol,  min_y, max_y));
      vertices_pc.push_back(GuiVertexPC(rx + radius, ry, color, botcol,  min_y, max_y));
      vertices_pc.push_back(GuiVertexPC(rx, ry, color, botcol,  min_y, max_y));


      /* top left corner */
      rx = x + radius;
      ry = y + radius;
      for (size_t i = 0; i < (points.size()/2)-1; ++i) {
      vertices_pc.push_back(GuiVertexPC(rx, ry, color, botcol,  min_y, max_y));
      vertices_pc.push_back(GuiVertexPC(rx + -points[i * 2 + 0],  (ry - points[i * 2 + 1]), color, botcol,  min_y, max_y));
      vertices_pc.push_back(GuiVertexPC(rx + -points[(i+1) * 2 + 0], (ry - points[(i+1) * 2 + 1]), color, botcol,  min_y, max_y));
    }

      /* fill left. */
      vertices_pc.push_back(GuiVertexPC(rx, ry, color, botcol,  min_y, max_y));
      vertices_pc.push_back(GuiVertexPC(rx - radius, ry, color, botcol,  min_y, max_y));
      vertices_pc.push_back(GuiVertexPC(rx - radius, (y + h - radius), color, botcol,  min_y, max_y));
      vertices_pc.push_back(GuiVertexPC(rx - radius, (y + h - radius), color, botcol,  min_y, max_y));
      vertices_pc.push_back(GuiVertexPC(rx, (y + h - radius), color, botcol,  min_y, max_y));
      vertices_pc.push_back(GuiVertexPC(rx, ry, color, botcol,  min_y, max_y));

      /* bottom left corner */
      rx = x + radius;
      ry = y + h - radius;
      for (size_t i = 0; i < (points.size()/2)-1; ++i) {
      vertices_pc.push_back(GuiVertexPC(rx, ry, color, botcol,  min_y, max_y));
      vertices_pc.push_back(GuiVertexPC(rx + -points[i * 2 + 0],  ry + points[i * 2 + 1], color, botcol,  min_y, max_y));
      vertices_pc.push_back(GuiVertexPC(rx + -points[(i+1) * 2 + 0], ry + points[(i+1) * 2 + 1], color, botcol,  min_y, max_y));
    }

      /* fill center */
      vertices_pc.push_back(GuiVertexPC(x + radius, y + h, color, botcol,  min_y, max_y));
      vertices_pc.push_back(GuiVertexPC(x + w - radius, y, color, botcol,  min_y, max_y));
      vertices_pc.push_back(GuiVertexPC(x + radius, y, color, botcol,  min_y, max_y));
      vertices_pc.push_back(GuiVertexPC(x + radius, y + h, color, botcol,  min_y, max_y));
      vertices_pc.push_back(GuiVertexPC(x + w - radius, y + h, color, botcol,  min_y, max_y));
      vertices_pc.push_back(GuiVertexPC(x + w - radius, y, color, botcol,  min_y, max_y));

      bg_counts.push_back(vertices_pc.size() - bg_offsets.back());

    }

      needs_update_pc = true;
#endif
    }

      // -------------------------------------------

      GuiVertexPC::GuiVertexPC() {
      setPos(0.0f, 0.0f);
      setColor(1.0f, 1.0f, 1.0f, 1.0f);
    }

      GuiVertexPC::GuiVertexPC(float x, float y, float* col) {
      setPos(x, y);
      setColor(col[0], col[1], col[2], col[3]);

#if 0
      float p = 1.0;
      float r, g, b, a;

      float shadetop = -0.15f;
      float shadebot = 0.15f;
      float coltop[3] = { 0 } ;
      float colbot[3] = { 0 };

      coltop[0] = std::min<float>(1.0f, col[0] + shadetop);
      coltop[1] = std::min<float>(1.0f, col[1] + shadetop);
      coltop[2] = std::min<float>(1.0f, col[2] + shadetop);
      colbot[0] = std::max<float>(0.0f, col[0] + shadebot);
      colbot[1] = std::max<float>(0.0f, col[1] + shadebot);
      colbot[2] = std::max<float>(0.0f, col[2] + shadebot);
  
      r = col[0];
      g = col[1];
      b = col[2];
      a = col[3];
      if (miny != 0.0f && maxy != 0.0f) {
      // printf("color: %f, %f, %f, botcol: %f, %f, %f, %p, %p\n", col[0], col[1], col[2], botcol[0], botcol[1], botcol[2], col, botcol);
      //printf("color: %f, %f, %f, botcol: %f, %f, %f, %p, %p\n", r, g, b, botcol[0], botcol[1], botcol[2], col, botcol);
      p = (maxy - y) / (maxy-miny);
      //    p *= 1.15;
      //    p *= p;


      /*
        r = (1.0 - p) * r + p * botcol[0];
        g = (1.0 - p) * g + p * botcol[1];
        b = (1.0 - p) * b + p * botcol[2];
        a = (1.0 - p) * a + p * botcol[3];
      */

      /*
        r = r + p * r + (1.0 - p) * botcol[0];
        g = g + p * g + (1.0 - p) * botcol[1];
        b = b + p * b + (1.0 - p) * botcol[2];
        a = a + p * a + (1.0 - p) * botcol[3];

        r = r * (p) + (1.0 - p) * botcol[0];
        g = g * (p) + (1.0 - p) * botcol[1];
        b = b * (p) + (1.0 - p) * botcol[2];
        a = a * (p) + (1.0 - p) * botcol[3];


  
        r = r + (p) * botcol[0];
        g = g + (p) * botcol[1];
        b = b + (p) * botcol[2];
        a = a + (p) * botcol[3];
      */
      r = (1.0 - p) * coltop[0] + p * colbot[0];
      g = (1.0 - p) * coltop[1] + p * colbot[1];
      b = (1.0 - p) * coltop[2] + p * colbot[2];

    
      //setColor(col[0], col[1], col[2], col[3]);
      //    printf("p: %f, %f, %f, %f\n", p, col[0], col[1], col[2]);
      }
      setPos(x, y);
      //  setColor(col[0], col[1], col[2], col[3]);
      //setColor(col[0], col[1], col[2], col[3]);
      setColor(r, g, b, a); //col[3]);
#endif
    }

      GuiVertexPC::GuiVertexPC(float x, float y, float r, float g, float b, float a) {
        setPos(x, y);
        setColor(r,g,b,a);
      }

      void GuiVertexPC::setPos(float x, float y) {
        pos[0] = x;
        pos[1] = y;
      }

      void GuiVertexPC::setColor(float r, float g, float b, float a) {
        color[0] = r;
        color[1] = g;
        color[2] = b;
        color[3] = a;
      }

      void GuiVertexPC::print() {
        printf("x: %3.2f, y: %3.2f, r: %3.2f, g: %3.2f, b: %3.2f a: %3.2f\n", pos[0], pos[1], color[0], color[1], color[2], color[3]);
      }

      // -------------------------------------------

      GuiVertexPT::GuiVertexPT() {
        setPos(0.0f, 0.0f);
        setTexCoord(0.0f, 0.0f);
      }

      GuiVertexPT::GuiVertexPT(float x, float y, float u, float v) {
        setPos(x,y);
        setTexCoord(u,v);
      }

      void GuiVertexPT::setPos(float x, float y) {
        pos[0] = x;
        pos[1] = y;
      }

      void GuiVertexPT::setTexCoord(float u, float v) {
        tex[0] = u;
        tex[1] = v;
      }

      // -------------------------------------------

      TextureDrawInfo::TextureDrawInfo()
        :offset(0)
        ,count(0)
        ,info(NULL)
      {
      }

      // -------------------------------------------

      TextureInfoGL::TextureInfoGL(GLenum type, int id)
        :id(id)
        ,type(type)
        ,tex_w(0)
        ,tex_h(0)
      {

        glBindTexture(type, id);
        glGetTexLevelParameteriv(type, 0, GL_TEXTURE_WIDTH, &tex_w);
        glGetTexLevelParameteriv(type, 0, GL_TEXTURE_HEIGHT, &tex_h);
      }

      int TextureInfoGL::getWidth() {
        return tex_w;
      }

      int TextureInfoGL::getHeight() {
        return tex_h;
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

  } // namespace rx

#endif
