#include <gui/Separator.h>
#include <gui/Render.h>
#include <gui/Group.h>

namespace rx { 

Separator::Separator(int height)
  :Widget(GUI_TYPE_SEPARATOR, "")
{
	h = height;
}

void Separator::create() {

  render->addRectangle(x, y, float(w - group->padding), float(h), group->bg_color, true, 0.0f, 0.1f);
}

} // namespace rx
