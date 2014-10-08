/*
  
  Container
  ----------
  
  The `Container` type is just used to hold a group of widgets and make
  sure that they are drawn/created when necessary. It's almost identical
  to a `Group` though the container doesn't add any graphical elements, 
  like the open/close header which created by the `Group` widget. 

  You use the `Container` type when you want to layour widgets freely.

  The container takes ownership of the added widgets and will free them
  when the container is freed itself. 

 */

#ifndef REMOXLY_GUI_CONTAINER_H
#define REMOXLY_GUI_CONTAINER_H


#include <string>
#include <gui/Types.h>
#include <gui/Widget.h>
#include <gui/Render.h>
#include <gui/Group.h>

namespace rx {

  class Container : public Widget {
  public:
    Container(Render* r);
    ~Container();
    Widget& add(Widget* wid);
    void draw();

  private:
    void update();
    void position();

  public:
    Render* render;                                /* The renderer which does all the drawing. */
    Group group;                                   /* Each widget need to be part of a group. We have one group that is used for this, but this group is not drawn. */
  };

  inline Widget& Container::add(Widget* wid) {

    wid->w = w;
    group.add(wid);
    return *wid;
  }

  inline void Container::update() {

    if (group.needsRedraw()) {
      group.render->clear();
      position();
      group.build();
      group.render->update();
    }
  }

  inline void Container::draw() {

    update();
    render->draw();
  }

  inline void Container::position() {

    for(std::vector<Widget*>::iterator it = group.children.begin(); it != group.children.end(); ++it) {

      Widget* wid = *it;
      if(wid->state & GUI_STATE_HIDDEN) {
        continue;
      }

      wid->position();
      wid->setBoundingBox();
    }
  }

} /* namespace rx */

#endif
