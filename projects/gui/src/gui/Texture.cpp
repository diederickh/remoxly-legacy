#include <gui/Texture.h>
#include <gui/Render.h>
#include <gui/Group.h>

Texture::Texture(std::string label, TextureInfo* info)
  :Widget(GUI_TYPE_TEXTURE, label)
  ,tex_info(info)
{
}

Texture::~Texture() {

  if(tex_info) {
    delete tex_info;
    tex_info = NULL;
  }
}

void Texture::setGroup(Group* g) {

  Widget::setGroup(g);

  float ratio = float(g->w) / tex_info->getWidth();
  h = (ratio * tex_info->getHeight());
}

void Texture::create() {
  render->addRectangle(x, y, w, h, group->bg_color, true);
  render->addRectangle(x, y, w, h, tex_info);
}

