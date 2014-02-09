/*

  Generator
  ---------

  The generator is used to create Panels, Groups, etc.. The generator 
  works closely with the Deserializer. 

 */

#ifndef REMOXLY_GUI_REMOTE_GENERATOR_H
#define REMOXLY_GUI_REMOTE_GENERATOR_H

#include <gui/Remoxly.h>

class Generator {
 public:
  virtual Panel* createPanel(int h = 200) = 0;                                                                 /* create a new panel with it's own renderer */
  virtual Group* createGroup(Panel* panel, std::string label) = 0;                                             /* create a new group on the given parent panel */
  virtual Group* createGroup(std::string label) = 0;                                                           /* create a new group with it's own renderer */
  virtual Slider<int>* createSliderInt(std::string label, int minv, int maxv, int step, int id) = 0;           /* create a slider int */
  virtual Slider<float>* createSliderFloat(std::string label, float minv, float maxv, float step, int id) = 0; /* create a slider float */
  virtual Toggle* createToggle(std::string label, int id) = 0;                                                 /* create a toggle */
  virtual ColorRGB* createColorRGB(std::string label, int id, int ncolors, float sat, float val) = 0;          /* create a color rgb slider */
  virtual Button* createButton(std::string label, int id, int buttonID, unsigned int icon) = 0;                /* create a button */
};

#endif
