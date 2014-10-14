/*

  Text 
  ------

  Text input

 */
#ifndef REMOXLY_GUI_SEPARATOR_H
#define REMOXLY_GUI_SEPARATOR_H

#include <gui/Widget.h>
#include <gui/Types.h>
#include <string>

namespace rx { 

class Separator : public Widget {

 public:
  Separator( int height = 14 );
  void create();

 public:
  int height;
};

} // namespace rx

#endif
