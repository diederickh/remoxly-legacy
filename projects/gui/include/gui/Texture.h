/*

  Texture
  --------

  Draws a texture.

 */
#ifndef REMOXLY_GUI_TEXTURE_H
#define REMOXLY_GUI_TEXTURE_H

#include <gui/Widget.h>
#include <gui/Types.h>
#include <string>

class Group;
class TextureInfo;

class Texture : public Widget {

 public:
  Texture(std::string label, TextureInfo* texinfo);     /* Texture takes ownership of the given TextureInfo. We will free it in the destructor. */
  ~Texture(); 
  void setGroup(Group* g);
  void create();

 public:
  TextureInfo* tex_info;
};

#endif
