/*

  ClientListener
  --------------

  Listens to changes from the Client object.

 */
#ifndef REMOXLY_GUI_REMOTE_CLIENT_LISTENER_H
#define REMOXLY_GUI_REMOTE_CLIENT_LISTENER_H

#include <string>

extern "C" { 
#  include <jansson.h>
}

namespace rx { 

class ClientListener { 

public:
  virtual void onTaskSetGuiModel(char* data, size_t len, std::string value) = 0; /* data is how we receive the data, value is the value for the given task */
  virtual void onDisconnected() = 0;
};

} // namespace rx

#endif
