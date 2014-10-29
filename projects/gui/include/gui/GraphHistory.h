/*

  Text 
  ------

  Text input

 */
#ifndef REMOXLY_GUI_GRAPH_HISTORY_H
#define REMOXLY_GUI_GRAPH_HISTORY_H

#include <gui/Widget.h>
#include <gui/Types.h>
#include <string>

namespace rx { 

class GraphHistory : public Widget {

 public:
  GraphHistory(std::string label, int size, float minv, float maxv, float height=166);
  void create();

//   /* interaction */
//   void onCharPress(unsigned int key);
//   void onKeyPress(int key, int modkeys);
//   void onMousePress(float mx, float my, int button, int modkeys);
//   void onMousePressOutside(float mx, float my, int button, int modkeys);
// 
//   /* text input specific */
//   void disableInputAndCopyValue();

  void addValue( float t );

 public:
  std::string& value;
  int text_x;
  int text_w;

    float       headValue;
	std::vector<float> verts;
    std::vector<float> values;
    int         headIndex, tailIndex;
    float       oneOverValuesSize;
    float       minv;
    float       maxv;
    float       oneOverMax;
        
    float       currMinValue;
    float       currMaxValue;
        
	bool		enableFPS;
    float       fpsTimeAccum;
    int         frameCounter;
    int			frameRate;
};

} // namespace rx

#endif
