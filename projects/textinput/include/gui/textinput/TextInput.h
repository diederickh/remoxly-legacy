
// ------------------------------------------------------------------------------
// H E A D E R
// ------------------------------------------------------------------------------

#ifndef ROXLU_TEXT_INPUT_H
#define ROXLU_TEXT_INPUT_H

#include <string>

#define TI_MODE_DISABLED 1
#define TI_MODE_INPUT    2
#define TI_MODE_SELECT   3

#define TI_MOD_NONE     0x0000
#define TI_MOD_SHIFT    0x0001
#define TI_MOD_CONTROL  0x0002
#define TI_MOD_ALT      0x0004

#define TI_KEY_SPACE     32
#define TI_KEY_BACKSPACE 259
#define TI_KEY_DELETE    261
#define TI_KEY_RIGHT     262
#define TI_KEY_LEFT      263

#define TI_NATIVE_KEY_ENTER				 13
#define TI_NATIVE_KEY_BACKSPACE			 8
#define TI_NATIVE_KEY_DELETE			 127
#define TI_NATIVE_KEY_LEFT				 276
#define TI_NATIVE_KEY_RIGHT				 275

#define TI_CURSOR_WIDTH  9                                        /* the default cursor width; only used when cursor is not behind a character */

#if defined(__APPLE__) && BITMAP_FONT_GL == BITMAP_FONT_GL2
#  define glGenVertexArrays glGenVertexArraysAPPLE
#  define glBindVertexArray glBindVertexArrayAPPLE
#endif

// ------------------------------------------------------------------------------
#if BITMAP_FONT_GL == BITMAP_FONT_GL2
static const char* TEXT_INPUT_VS = ""
  "#version 110\n"
  "#extension GL_EXT_gpu_shader4 : require\n"
  "attribute vec2 a_pos;"
  "uniform mat4 u_pm;"
  "uniform vec4 u_pos;"
  //  "vec2 pos[4];"

  "void main() {"
#if 0
  "  pos[0] = vec2(0.0, 0.0);"
  "  pos[1] = vec2(0.0, 1.0);"
  "  pos[2] = vec2(1.0, 0.0);"
  "  pos[3] = vec2(1.0, 1.0);"
  "  vec2 p = pos[gl_VertexID];"
#else 
  "  vec2 p = a_pos; " 
#endif
  "  p.x *= u_pos.z;"
  "  p.y *= u_pos.w;"
  "  p   += u_pos.xy;"
  "  gl_Position = u_pm * vec4(p, 0.0, 1.0);"
  "}"
  "";

static const char* TEXT_INPUT_FS = "" 
  "#version 110\n"
   "void main() {"
  "  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
  "}"
  "";
#endif

// ------------------------------------------------------------------------------

#if BITMAP_FONT_GL == BITMAP_FONT_GL3
static const char* TEXT_INPUT_VS = ""
  "#version 150\n"
  "uniform mat4 u_pm;"
  "uniform vec4 u_pos;"
  "const vec2[4] pos = vec2[]("
  "      vec2(0.0, 0.0), "
  "      vec2(0.0, 1.0), "
  "      vec2(1.0, 0.0), "
  "      vec2(1.0, 1.0)  "
  ");"
  "void main() {"
  "  vec2 p = pos[gl_VertexID];"
  "  p.x *= u_pos.z;"
  "  p.y *= u_pos.w;"
  "  p   += u_pos.xy;"
  "  gl_Position = u_pm * vec4(p, 0.0, 1.0);"
  "}"
  "";

static const char* TEXT_INPUT_FS = "" 
  "#version 150\n"
  "out vec4 fragcolor;"
  "void main() {"
  "  fragcolor = vec4(1.0, 0.0, 0.0, 1.0);"
  "}"
  "";
#endif

// ------------------------------------------------------------------------------
namespace rx { 

class TextInput {

 public:
  TextInput(float x, float y, float w, BitmapFont& font);                      /* create the textinput for the given font at x/y with a maximum width of `w` */
  void onCharPress(unsigned int key);                                          /* call this whenever a plain ascii character is pressed */
  void onKeyPress(int key, int mods = TI_MOD_NONE);                            /* call this whenever one of the special keys (TI_KEY_*) is pressed */
  void setValue(std::string value);                                            /* fill the input with ascii values, make sure that you've enable()'d the input else this won't work */
  std::string getValue();                                                      /* returns the current contents of the input (same as the contents member) */
  void draw();                                                                 /* draw the current contents to screen */
  void select();                                                               /* selects all text and changes the mode to TI_MODE_SELECT; hitting backspace or delete will remote all contents. arrows move to complete left or complete right */
  void deselect();                                                             /* deselect the text and switch back to TI_MODE_INPUT */
  void clear();                                                                /* clears all contents, resets the text input */
  void enable();                                                               /* changes the mode to TI_MODE_INPUT which allows new characters to be added */
  void disable();                                                              /* disables the input; you won't be able to add any more text */

 private:
  bool setupGraphics();                                                        /* sets up opengl state (only once) */
  void updateContents();                                                       /* should be called whenever the contents changes */
  void updateCursor();                                                         /* should be called whenever the cursor changes position */
  void removeCharacterAtCurrentPosition();                                     /* delete */
  void removeCharacterAtPrevPosition();                                        /* backspace */
  bool canMoveCursorToLeft();                                                  /* returns true when we can move the cursor to the left (e.g. there are characters) */
  bool moveCursorToLeft();                                                     /* returns true when moved to the left */
  bool moveCursorToRight();                                                    /* returns true when moved to the right */
  void moveCursorToMostLeft();                                                 /* moves the cursor to the most left, done when we're in select mode and the right key has been pressed */
  void moveCursorToMostRight();                                                /* moves the cursor to the most right. */
  bool canAddContent();                                                        /* returns true when we can add content to the input; must have a correct mode and the width cannot exceed the given `w */
  bool canRemoveContent();                                                     /* returns true when we can remove contents from the string and the contents is allowed to be changed */
                                                                               
 public:                                                                       
  BitmapFont& font;                                                            
  int mode;                                                                    /* the current mode; certain keys react differently in a certain mode */
  float x;                                                                     /* x-position where we draw */
  float y;                                                                     /* y-position where we draw */
  float w;                                                                     /* the max width of this input */
  int char_dx;                                                                 /* the current active character index, used with delete/backspace */
  int cursor_x;                                                                /* x position of cursor, relative to `x` */
  int cursor_w;                                                                /* width for the cursor, for the current character, or default width when the cursor is position at the end of the string */
  float content_w;                                                             /* the width of the current content; cannot exceed the `w` member. */
  int align;
  std::string contents;

  /* opengl */
  static bool is_initialized;
  static GLuint prog;
  static GLuint vert;
  static GLuint frag;
  static GLuint vao;
  static GLint u_pos;

# if BITMAP_FONT_GL == BITMAP_FONT_GL2
  static GLuint vbo; 
# endif

};

} // namespace 

#endif // ROXLU_TEXT_INPUT

// ------------------------------------------------------------------------------
// I M P L E M E N T A T I O N
// ------------------------------------------------------------------------------

#if defined(TEXT_INPUT_IMPLEMENTATION)

namespace rx { 

bool TextInput::is_initialized = false;
GLuint TextInput::prog = 0;
GLuint TextInput::vert = 0;
GLuint TextInput::frag = 0;
GLuint TextInput::vao = 0;
GLint TextInput::u_pos = -1;

#if BITMAP_FONT_GL == BITMAP_FONT_GL2
GLuint TextInput::vbo = 0;
#endif

TextInput::TextInput(float x, float y, float w, BitmapFont& font)
  :mode(TI_MODE_DISABLED)
  ,x(x)
  ,y(y)
  ,w(w)
  ,font(font)
  ,char_dx(0)
  ,cursor_x(0)
  ,cursor_w(TI_CURSOR_WIDTH)
  ,content_w(0)
  ,align(BITMAP_FONT_ALIGN_LEFT)
{
  if(!TextInput::is_initialized) {

    glGenVertexArrays(1, &vao);

#if BITMAP_FONT_GL == BITMAP_FONT_GL2
    float caret_pos[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0 } ;

    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(caret_pos), caret_pos, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (GLvoid*)0);
#endif

    // shader
    vert = bitmapfont_create_shader(GL_VERTEX_SHADER, TEXT_INPUT_VS);
    frag = bitmapfont_create_shader(GL_FRAGMENT_SHADER, TEXT_INPUT_FS);

#if BITMAP_FONT_GL == BITMAP_FONT_GL2
    const char* atts[] = { "a_pos" } ;
    prog = bitmapfont_create_program(vert, frag, 1, atts);
#else
    prog = bitmapfont_create_program(vert, frag);
#endif

    // ortho matrix
    GLint viewport[4];
    float pm[16];
    glGetIntegerv(GL_VIEWPORT, viewport);
    bitmapfont_ortho(0.0f, viewport[2], viewport[3], 0.0f, 0.0f, 100.0f, pm);

    glUseProgram(prog);
    glUniformMatrix4fv(glGetUniformLocation(prog, "u_pm"), 1, GL_FALSE, pm);

    u_pos = glGetUniformLocation(prog, "u_pos");
    
    TextInput::is_initialized = true;
  }
}

void TextInput::setValue(std::string value) {
  for(size_t i = 0; i < value.size(); ++i) {
    onCharPress(value[i]);
  }
}

std::string TextInput::getValue() {
  return contents;
}

void TextInput::onCharPress(unsigned int key) {

  if( ((key > 126) || (key < 33) ) && key != TI_KEY_SPACE) {
    return;
  }

  if(mode == TI_MODE_SELECT) {
    clear();
    mode = TI_MODE_INPUT;
  }

  if(!canAddContent()) {
    printf("Warning: cannot add content to text input (mode: %d, char: %c, this %p).\n", mode, key, this);
    return;
  }

  // do we have the character in the font?
  Character ch;

  if(!font.getChar(key, ch)) {
    printf("Error: Character not found in font: %d\n", key);
    return;
  }

  // will the new character exceed out width?
  if((ch.xadvance + content_w) > w) {
    return;
  }

  contents.insert(char_dx, 1, (char)key);

  moveCursorToRight();
  updateContents();
}

void TextInput::onKeyPress(int key, int mods) {

  if(mode == TI_MODE_INPUT) {

    switch(key) {
	  case TI_NATIVE_KEY_BACKSPACE:
      case TI_KEY_BACKSPACE: {
        removeCharacterAtPrevPosition();
        break;
      }
      case TI_NATIVE_KEY_DELETE:
      case TI_KEY_DELETE: {
        removeCharacterAtCurrentPosition();
        break;
      }
	  case TI_NATIVE_KEY_LEFT:
      case TI_KEY_LEFT: {
        moveCursorToLeft();
        break;
      }
	  case TI_NATIVE_KEY_RIGHT:
      case TI_KEY_RIGHT: {
        moveCursorToRight();
      }
      default:break;
    }
  }
  else if(mode == TI_MODE_SELECT) {
    switch(key) {
	  case TI_NATIVE_KEY_BACKSPACE:
      case TI_KEY_DELETE:
      case TI_KEY_BACKSPACE: {
        clear();
        mode = TI_MODE_INPUT;
        break;
      }
	  case TI_NATIVE_KEY_LEFT:
      case TI_KEY_LEFT: {
        moveCursorToMostLeft();
        deselect();
        break;
      }
	  case TI_NATIVE_KEY_RIGHT:
      case TI_KEY_RIGHT: {
        moveCursorToMostRight();
        deselect();
        break;
      }
      default:break;
    }
  }
}

void TextInput::updateContents() {
  font.clear();
  font.write(x, y, contents, align);
  content_w = font.getWidth(contents);
}

bool TextInput::canAddContent() {
  return (mode != TI_MODE_DISABLED && content_w < w);
}

bool TextInput::canRemoveContent() {
  return mode != TI_MODE_DISABLED;
}

void TextInput::updateCursor() {

  // cursor is outside the string
  if(char_dx >= contents.size()) {

    if(align == BITMAP_FONT_ALIGN_LEFT) {
      cursor_x = font.getWidth(contents);
    }
    else if(align == BITMAP_FONT_ALIGN_RIGHT) {
      cursor_x = 0;
      cursor_w = TI_CURSOR_WIDTH;
    }
  }
  // cursor inside the string.
  else if (char_dx >= 0) {

    if(align == BITMAP_FONT_ALIGN_LEFT) {
      std::string sub = contents.substr(0, char_dx);
      cursor_x = font.getWidth(sub); 
    }
    else if(align == BITMAP_FONT_ALIGN_RIGHT) {
      std::string sub = contents.substr(char_dx, contents.size() - char_dx);
      cursor_x = -font.getWidth(sub); 
    }

    Character ch;
    if(font.getChar(contents[ char_dx ], ch)) {
      cursor_w = ch.xadvance; 
    }
    else {
      printf("Error: Cannot find current char.\n");
    }
  }
}

bool TextInput::canMoveCursorToLeft() {
  return char_dx > 0;
}

bool TextInput::moveCursorToLeft() {

  char_dx--;

  if(char_dx < 0) {
    char_dx = 0;
    return false;
  }

  updateCursor();

  return true;
}

bool TextInput::moveCursorToRight() {

  char_dx++;

  if(char_dx > contents.size()) {
    char_dx = contents.size();
    return false;
  }

  updateCursor();

  return true;
}

void TextInput::moveCursorToMostLeft() {

  cursor_x = 0;
  char_dx = 0;
  cursor_w = TI_CURSOR_WIDTH;

  Character ch;
  if(contents.size() && font.getChar(contents[0], ch)) {
    cursor_w = ch.xadvance;
  }
  
  updateCursor();
}

void TextInput::moveCursorToMostRight() {
  char_dx = contents.size();
  updateCursor();
}

void TextInput::removeCharacterAtCurrentPosition() {

  if(char_dx >= contents.size()) {
    return;
  }

  if(contents.size()) {
    contents.erase(contents.begin() + char_dx);
    updateContents();
    updateCursor();
  }
}

void TextInput::removeCharacterAtPrevPosition() {

  if(canMoveCursorToLeft()) {
    contents.erase(contents.begin() + (char_dx-1));
    updateContents();
    moveCursorToLeft();
  }
}

void TextInput::select() {

  if(contents.size()) {
    mode = TI_MODE_SELECT;
  }
}

void TextInput::deselect() {
  mode = TI_MODE_INPUT;
}

void TextInput::enable() {
  mode = TI_MODE_INPUT;
}

void TextInput::disable() {
  mode = TI_MODE_DISABLED;
}

void TextInput::clear() {

  contents.clear();

  char_dx = 0;
  cursor_x = 0;
  cursor_w = TI_CURSOR_WIDTH;
  content_w = 0;
  
  updateContents();
  updateCursor();
}

void TextInput::draw() {

  if(mode == TI_MODE_DISABLED) {
    return;
  }

  //glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // premultiplied alpha
  //glBlendFunc(GL_ONE, GL_ONE);

  glBindVertexArray(vao);
  glUseProgram(prog);

  if(mode == TI_MODE_INPUT) {
    glUniform4f(u_pos, x + cursor_x, y, cursor_w, font.line_height);
  }
  else if(mode == TI_MODE_SELECT) {
    int m = (align == BITMAP_FONT_ALIGN_RIGHT) ? -1 : 1;
    glUniform4f(u_pos, x, y, content_w * m, font.line_height);
  }

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  font.draw();
}

} // namespace 
#endif // TEXT_INPUT_IMPLEMENTATION
