/*

  Widget
  ------

  A widget is a element that can be added to a Gui/Panel. 
  See the comments behind each of the functions below. 

  Notes:
  ------

  - In each widget can create the thing they want to draw in the 
  create() function.

  - If you have a composite widget, where some of the "sub" widgets
  are not added to the children vector, you need to make sure that
  you override `unsetNeedsRedraw()` and set the `needs_redraw` to 
  false for all children. See the Slider widget for an example of
  where we do exactly that.

*/

#ifndef REMOXLY_GUI_WIDGET_H
#define REMOXLY_GUI_WIDGET_H

#include <vector>
#include <string>
#include <stdint.h>
#include <gui/Types.h>

namespace rx { 

  class Group;
  class Render;
  class WidgetListener;

  class Widget {

  public:
    Widget(int type, std::string label);
    virtual ~Widget();
    virtual void build();                                            /* build will check if this widget is allowed to be drawn, and if so it will call create() and create() on all of its children */
    virtual void create();                                           /* create the shapes; when needs_redraw is set to true this will be called again */
    virtual void add(Widget* w, Group* g);                           /* add a child element */
    virtual void position();                                         /* calculate the x/y/w/h etc.. values for the widget itself and for its children. note that x/y will be set by the container which is probably Gui. position() may only be called by the parent container after the x/y are set. */
    virtual void setGroup(Group* g);                                 /* setup the widget. sets the gui + render members by default, a user shouldn't call this */
    virtual void setBoundingBox();                                   /* calculate the bounding box of this element */
    virtual void print();                                            /* prints some debug info */

    /* layout. */
    virtual Widget& setPosition(float px, float py);                 /* sets the position of the widget, e.g. can be used with the `Container` to freely position widgets. */
    virtual Widget& setSize(float pw, float ph);                     /* set the size of the widget. */ 
    virtual Widget& setWidth(float pw);                              /* set the width of the widget. */
    virtual Widget& setHeight(float ph);                             /* set the height of the widget. */

    /* state management */
    virtual void lockPosition();                                     /* lock the position. when the position is locked the widget is not allowed to change the x/y itself */
    virtual void unlockPosition();                                   /* unlock the position */
    virtual void hide();                                             /* adds the hide flag, the element wont be drawn, or won't receive interaction */
    virtual void hideChildren();                                     /* hide all children */
    virtual void show();                                             /* show the element */
    virtual void showChildren();                                     /* show all children */
    virtual void open();                                             /* open the widget (and containing children, see openChildren ), but still do not show hidden elements */
    virtual void openChildren();                                     /* call open() on all children */
    virtual void close();                                            /* close the widget (and containing children, see closeChildren) */
    virtual void closeChildren();                                    /* call close() on all children */
    virtual bool isDrawn();                                          /* checks if the state is CLOSED or HIDDEN. if so, we will not use this widget */

    virtual void unsetNeedsRedraw();                                 /* after calling create we will call this on all the elements. the goal of this function is to set "needs_redraw = false". This should also be done for all other elements which are not added as children but are part of the widget */
    virtual void unsetNeedsRedrawChildren();                         /* calls unsetNeedsRedraw() on all children */

    /* interaction */
    virtual void onCharPress(unsigned int key);                                               /* gets called for ascii characters */
    virtual void onKeyPress(int key, int modkeys);                                            /* gets called with scancodes + mods */
    virtual void onKeyRelease(int key, int modkeys);
    virtual void onMousePress(float mx, float my, int button, int modkeys);                   /* gets called when the mouse is pressed */ 
    virtual void onMousePressOutside(float mx, float my, int button, int modkeys);            /* gets called when the mouse is pressed outside the "container" that wraps around this widget. we needs this function so you can disable e.g. the number input when the user pressed outside the "clickable" area of the container. w/o this the Panel + scroll bar won't have a way to prevent mouse clicks outside the visible area. See the slider widget. This function should only be used to disable a certain state (e.g. disable editing state) */
    virtual void onMouseRelease(float mx, float my, int button, int modkeys);                 /* gets called when the mouse is released */
    virtual void onMouseReleaseOutside(float mx, float my, int button, int modkeys);          /* gets called when the mouse is released outside the "container" that wraps around this widget. we needs this function so you can disable e.g. the number input when the user pressed outside the "clickable" area of the container. w/o this the Panel + scroll bar won't have a way to prevent mouse releases outside the visible area. See the slider widget. This function should only be used to disable a certain state (e.g. disable editing state) */
    virtual void onMouseMove(float mx, float my);                                             /* gets called when the mouse is moved */

    /* forward interaction to children */
    virtual void onCharPressChildren(unsigned int key);
    virtual void onKeyPressChildren(int key, int modskeys);
    virtual void onKeyReleaseChildren(int key, int modskeys);
    virtual void onMouseReleaseChildren(float mx, float my, int button, int modkeys);          /* calls onRelease() on all children */
    virtual void onMouseReleaseOutsideChildren(float mx, float my, int button, int modkeys);   /* calls onReleaseOutside() on all chilren */
    virtual void onMousePressChildren(float mx, float my, int button, int modkeys);            /* calls onPress() on all children */
    virtual void onMousePressOutsideChildren(float mx, float my, int button, int modkeys);     /* calls onPressOutSide() on all children */
    virtual void onMouseMoveChildren(float mx, float my);                                      /* calls onMove() on all children */
  
    /* functions operating on all children */
    virtual void removeChildren();                                   /* cleans up the children vector. deletes them all */
    virtual bool needsRedraw();                                      /* checks if the widget needs to be recreated/redrawn, checks itself and iterates over all children and returns true as soon as one needs to be redrawn. */
    virtual bool needsRedrawChildren();                              /* checks if the children need to be redrawn */
    virtual void buildChildren();                                    /* call create() on this element and scalls all create() functions of the child elements of the widget */
    void setBoundingBoxChildren();                                   /* calculate bounding boxes for all children */

    /* events and listeners */
    virtual void addListener(WidgetListener* l);                     /* add a listener for this widget - will call addListenerChildren() too */
    virtual void addListenerChildren(WidgetListener* l);             /* adds the listeners to the children of this elements - called from addListener() */
    virtual void disableNotifications();
    virtual void enableNotifications();
    virtual void notify(int event);                                  /* notify event listeners when something changes - will call notifyChildren() too */

  public:
    Group* group;                                                    /* the group in which this widget is contained */
    Render* render;                                                  /* the renderer which takes care of the drawing */
    static uint32_t generated_ids;                                   /* each widget is assigned an auto incremented unique ID. this can be used when doing networked guis */
    uint32_t id;                                                     /* the unique ID assigned to this widget */
    int type;                                                        /* the widget type, see Types.h */
    int state;                                                       /* the current state of the widget, see Types.h for available states */
    int mods;                                                        /* the mods that were pressed on mouse down, will be unsed in onRelease(). This can be used to have a different kind of interaction when someone e.g. drags with SHIFT down. */
    int style;                                                       /* bitset that is used to toggle certain styles, like which rounded corner to draw, draw outline, draw drop shadow etc.. */
    int x;                                                           /* (absolute) x position of this widget */
    int y;                                                           /* (absolute) y position of this widget */
    int w;                                                           /* the width of the visual elements of this widget */
    int h;                                                           /* the height of the visual elements of this widget */
    int bbox[4];                                                     /* bounding box of the visual elements that this widget contains. must be set in setBoundingBox(), 0 = top left x, 1 = top left y, 2 = width, 3 = height */
    bool needs_redraw;                                               /* this flag is used in needsRedraw() by default (if you didn't override this function). when you set needs_redraw to true your create() function will be called during the next render */
    std::string label;                                               /* the label for this element */
    std::vector<Widget*> children;
    std::vector<WidgetListener*> listeners;

    /* mouse interaction */
    int mouse_x;                                                     /* the "last" mouse x. is set in onPress/onRelease/onMove */
    int mouse_y;                                                     /* the "last" mouse y. is set in onPress/onRelease/onMove */
    int mouse_press_x;                                               /* the x position where got a onPress, is reset in onRelease() */
    int mouse_press_y;                                               /* the y position where we got a onPress(), is reset in onRelease */
    int press_x;                                                     /* the x position of the widget when the press began */
    int press_y;                                                     /* the y position of the widget when the press began */
  };

  /* -------------------------------------------------------------------------------------------------------------- */

  inline bool Widget::isDrawn() {
    return !(state & GUI_STATE_UNUSED);
  }

  inline Widget& Widget::setPosition(float px, float py) {
    x = px;
    y = py;
    return *this;
  }

  inline Widget& Widget::setSize(float pw, float ph) {
    setWidth(pw);
    setHeight(ph);
    return *this;
  }

  inline Widget& Widget::setWidth(float pw) {
    w = pw;
    return *this;
  }

  inline Widget& Widget::setHeight(float ph) {
    h = ph;
    return *this;
  }


} // namespace rx

#endif
