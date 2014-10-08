#include <gui/Container.h>

namespace rx {

  Container::Container(Render* r)
    :Widget(GUI_TYPE_CONTAINER, "")
    ,render(r)
    ,group("", r)
  {

    x = 10;
    y = 10;
    w = 275;
    h = 22;

    if (NULL == r) {
      printf("Error: the given Render pointer is NULL.");
      exit(1);
    }

    group.hideHeader();
    Widget::add(&group, &group);
  }

  Container::~Container() {
    group.removeChildren();
  }

} /* namespace rx */
