/*
  
  Scroll
  ------

  This class takes care of drawing a scrollbar and doing the 
  necessary math which provides the user with all the values 
  needed to use glScissor().
  
  Used terms:
  ----------
  
  - grip: this is the handle that you use to drag in the track area
  - track: the track is the "area" where the grip can move 
  - visible area: the "window" for what you see. 

  Implementation info
  --------------------
  
  - GUI_STATE_DOWN_INSIDE:    Used to handle state of the grip
  - GUI_STATE_DOWN_CUSTOM0:   Used to handle state of the track bar.

 */

#ifndef REMOXLY_SCROLL_H
#define REMOXLY_SCROLL_H

#include <gui/Widget.h>
#include <gui/IconButton.h>
#include <gui/Utils.h>

// -------------------------------------------

static void scroll_up_click(int id, void* user);
static void scroll_down_click(int id, void* user);

// -------------------------------------------

class Scroll : public Widget {

 public:
  Scroll();
  bool setup(Gui* g);
  void create();
  void beginScissor();
  void endScissor();
  void setVisibleArea(int visx, int visy, int visw, int vish, int contenth);  /* pass in the visible_{x,y,w,h} and the height of the actual content that needs to be scrolled */
  void position();                                                            /* calculates all positions and offsets */

  /* interaction */
  void onMousePress(float mx, float my, int button, int modkeys);
  void onMouseRelease(float mx, float my, int button, int modkeys);
  void onMouseMove(float mx, float my);

  /* scroll specific */
  void setMousePositionValue(float my);            /* used to offset the grip position based on the given mouse y */
  void setPercentageValue(float p);                /* calculates the grip position 0 = align with top, 1 = align with bottom */
  void movePageUp();                               /* this will move the contents "up", a page is the height of the visible area */
  void movePageDown();                             /* this will move the contents "down", "" "" */

 public:
  float perc;                                      /* how much we should offset the contents in percentages. */
                                                   
  /* the visible window */                         
  int visible_x;                                   /* most left x which should be visible */
  int visible_y;                                   /* the top left most  y which should be visible */
  int visible_w;                                   /* the width of the visible area, aka width of the window */
  int visible_h;                                   /* the height of the visible area, aka height of the window */
                                                   
  /* content */                                    
  int content_h;                                   /* the height of the content which is supposed to scroll */
  int prev_content_h;
  int offset_y;                                    /* the amount that the content should be move "up" */
                                                   
  /* track and grip */                             
  int grip_y;                                      /* the y position where we start drawing the grip */
  int grip_h;                                      /* the current height of the grip, calculated using the content/visible ratio */
  int grip_space;                                  /* the number of pixels the grip can move */
  int track_y;                                     /* where the track starts, the grip slides inside this track */
  int track_h;                                     /* the height of the tracking area */
  int scroll_bar_x;                                /* the x where we start drawing the scroll bar */
  int scroll_bar_y;                                /* the top most y where were start drawing the scroll bar */

  /* interaction */
  IconButton up_button;
  IconButton down_button;
};

#endif
