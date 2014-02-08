#ifndef REMOXLY_GUI_REMOTE_UTILS_H
#define REMOXLY_GUI_REMOTE_UTILS_H

#include <string>
#include <stdlib.h>
#include <stdint.h>

#if defined(__APPLE__)
#  include <mach/mach_time.h>
#endif

extern "C" {
#  include <libwebsockets.h>
#  include <jansson.h>
}

#define REMOXLY_FREE_JSON(el) { if(el) { json_decref(el); el = NULL; } el = NULL; } 

std::string remoxly_websocket_reason_to_string(enum libwebsocket_callback_reasons reason);

int remoxly_websocket_write(struct libwebsocket* ws, 
                            unsigned char* data, 
                            size_t len, 
                            int flag = LWS_WRITE_TEXT);

bool remoxly_json_get_float(json_t* el, const std::string& name, float& result);
bool remoxly_json_get_int(json_t* el, const std::string& name, int& result);
bool remoxly_json_get_string(json_t* el, const std::string& name, std::string& result);


// high resolution time
uint64_t remoxly_hrtime();

// -----------------------------------------------------------

// Used by Server and Client, it describe a task they need to process
struct ConnectionTask {
  ConnectionTask();
  int task_name;
  int task_id;
  std::string task_data;
};

// -----------------------------------------------------------

#endif
