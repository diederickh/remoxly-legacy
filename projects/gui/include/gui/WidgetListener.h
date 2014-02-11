/*

  Listener
  --------

  Listeners are used to be notified about changes in widgets. Listeners
  were created for the remoteing part of Remoxly. Clients need to know
  when a value has changed; this is done with listeners. 
  Besides the networking it's up to the user how use these. 
  See Types.h for the events that can be fired.

 */
#ifndef REMOXLY_GUI_LISTENER_H
#define REMOXLY_GUI_LISTENER_H

class Widget;

class WidgetListener {

 public:
  virtual void onEvent(int event, Widget* w) = 0;
};

#endif
