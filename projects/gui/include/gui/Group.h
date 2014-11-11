/*

  Group
  ----

  The Group this is a collection of widgets with on top a header. The 
  header can be used to drag around the contains widgets. 

  IMPORTANT:
  ----------

  The "h" value represents the height of the header we draw; it's not the
  height of all children. This can be fetched using the bounding box (see
  the bbox member).

*/
#ifndef REMOXLY_GUI_GROUP_H
#define REMOXLY_GUI_GROUP_H

#include <string>
#include <gui/Types.h>
#include <gui/Widget.h>
#include <gui/Render.h>
#include <gui/IconButton.h>

namespace rx { 

  /* -------------------------------------------------------------------------------------------------------------- */

  static void group_close_click(int id, void* user);
  static void group_open_click(int id, void* user);

  /* -------------------------------------------------------------------------------------------------------------- */

  class Group : public Widget {

  public:
    Group(std::string label, Render* r, int style = GUI_CORNER_ALL);
    ~Group();

    void add(Widget* wid);                                       /* add a widget to the group */
    void draw();                                                 /* will draw the group */
    void create();                                               /* is called when you need to the shapes for the element(s) */
    void position();
    void hideHeader();                                           /* hide the header */
    void showHeader();                                           /* show the header (default) */

    /* interaction */
    void onMousePress(float mx, float my, int button, int mods);
    void onMouseRelease(float mx, float my, int button, int mods);
    void onMouseMove(float mx, float my);
    bool needsRedraw();
    void unsetNeedsRedraw();

    /* colors */
    void setColors(float* bg, float* fg, float* highlight);                         /* sets the colors for the current theme */
    float* getStateColor(Widget* w, int flag, float* off, float* on);               /* this will return either the on or off color depending if the given flag is set in the state member of the widget. you can use this to switch between highlight color and default color */
    float* getButtonStateColor(Widget* w, int flag = GUI_STATE_DOWN_INSIDE);        /* returns the color for the button based on the current state. by default it will return the standard button color when the flag is not set. if it's set it will return the highlight color */
    float* getButtonLabelStateColor(Widget* w, int flag = GUI_STATE_DOWN_INSIDE);   /* returns the color for the label of a button. */
    float* getForegroundStateColor(Widget* w, int flag = GUI_STATE_DOWN_INSIDE);    /* returns the color for "hightlight" elements. this is for example the part of a slider which indicates how much is "slided" */
    float* getBackgroundStateColor(Widget* w, int flag = GUI_STATE_DOWN_INSIDE);    /* returns the background color. you could hightlight a background color too, which is e.g. used in the scroll bar */
    float* getSelectedStateColor(Widget* w, int flag = GUI_STATE_DOWN_INSIDE);      /* returns the highlight color when the mouse is down inside the element, else it returns the default selected color. */ 


  public:
    void update();                                                               /* should not be called by the user! */

  public:
    Render* render;
    bool show_header;

    /* alignment */
    int padding;
    int xindent; 
    int yindent;

    /* theme: colors, shades */
    float fg_color[4];              /* foreground color; use this for e.g. fills */
    float bg_color[4];              /* background color; use this for e.g. backgrounds */
    float line_color[4];            /* outline colors. */
    float label_color[4];           /* a label color should be used for text labels */
    float number_color[4];          /* used when drawing numeric values */
    float button_color[4];          /* color for buttons, based on the color sheme */
    float highlight_color[4];       /* e.g. for button highlight colors */
    float panel_color[4];           /* this background of the complete group */
    float header_color[4];          /* color of the header above a group (with the open/close feature) */
    float selected_color[4];        /* color of a selected item (e.g. fill of slider, or a popup item) */
    float text_bg_color[4];         /* backround for text inputs (or number) */
    float shade_top;                /* how many shading to apply to the top of elements, -0.1 wil make it darker, 0.1 will make it brighter. */
    float shade_bottom;             /* how many shading to apply to the bottom of elements, -0.1 will make it darker, 0.1 will make it brighter. */

    /* interaction */
    IconButton close_button;
    IconButton open_button;
  };

  /* -------------------------------------------------------------------------------------------------------------- */

  inline void Group::showHeader() {
    show_header = true;
    h = 22;
  }

  inline void Group::hideHeader() {
    show_header = false;
    h = 0;
  }

}  // namespace rx

#endif
