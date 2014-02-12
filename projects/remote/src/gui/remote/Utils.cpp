#include <gui/remote/Utils.h>

namespace rx { 

// -----------------------------------------------------------

int remoxly_websocket_write(struct libwebsocket* ws, 
                          unsigned char* data, 
                          size_t len, 
                          int flag)
{

  int n = libwebsocket_write(ws, data, len, LWS_WRITE_TEXT);

  if(n < 0) {
    printf("Error: libwebsocket_write, returned %d when trying to send gui model to client.", n);
    return -1;
  }

  if(n < 1) {
    printf("Error: partial write when trying to send data to the sever.\n");
    return -1;
  }

  return 0;
}

// -----------------------------------------------------------

bool remoxly_json_get_float(json_t* el, const std::string& name, float& result) {

  if(!el) {
    printf("Error: cannot get float; json_t* is invalid.\n");
    return false;
  }

  json_t* val = json_object_get(el, name.c_str());
  if(!val) {
    printf("Error: cannot get the object `%s` from the given json element.\n", name.c_str());
    return false;
  }

  if(!json_is_number(val)) {
    printf("Error: the value for `%s` is not a number. Cannot get float value.\n", name.c_str());
    return false;
  }

  result = (float) json_real_value(val);

  return true;
}

bool remoxly_json_get_int(json_t* el, const std::string& name, int& result) {

  if(!el) {
    printf("Error: cannot get int; json_t* is invalid.\n");
    return false;
  }

  json_t* val = json_object_get(el, name.c_str());
  if(!val) {
    printf("Error: cannot get the object `%s` from the given json element.\n", name.c_str());
    return false;
  }

  if(!json_is_number(val)) {
    printf("Error: the value for `%s` is not a number. Cannot get float value.\n", name.c_str());
    return false;
  }

  result = json_integer_value(val);

  return true;
}


bool remoxly_json_get_string(json_t* el, const std::string& name, std::string& result) {

  if(!el) {
    printf("Error: cannot get string; json_t* is invalid.\n");
    return false;
  }

  json_t* val = json_object_get(el, name.c_str());
  if(!val) {
    printf("Error: cannot get the object `%s` from the given json element.\n", name.c_str());
    return false;
  }

  if(!json_is_string(val)) {
    printf("Error: not a string value.\n");
    return false;
  }

  result = json_string_value(val);

  return true;
}

uint64_t remoxly_hrtime() {
#if defined(__APPLE__) 
  mach_timebase_info_data_t info;
  if(mach_timebase_info(&info) != KERN_SUCCESS) {
  abort();
}
  return mach_absolute_time() * info.numer / info.denom;

#elif defined(__linux)
  static clock_t fast_clock_id = -1;
  struct timespec t;
  clock_t clock_id;

  if(fast_clock_id == -1) {
    if(clock_getres(CLOCK_MONOTONIC_COARSE, &t) == 0 && t.tv_nsec <= 1 * 1000 * 1000LLU) {
      fast_clock_id = CLOCK_MONOTONIC_COARSE;
    }
    else {
      fast_clock_id = CLOCK_MONOTONIC;
    }
  }

  clock_id =  CLOCK_MONOTONIC;
  if(clock_gettime(clock_id, &t)) {
    return 0; 
  }

  return t.tv_sec * (uint64_t)1e9 +t.tv_nsec;

#elif defined(_WIN32)
  LARGE_INTEGER timer_freq;
  LARGE_INTEGER timer_time;
  QueryPerformanceCounter(&timer_time);
  QueryPerformanceFrequency(&timer_freq);
  static double freq = (double)timer_freq.QuadPart/(double)1000000000;
  return (uint64_t)((double)timer_time.QuadPart / freq);
#endif
};

// -----------------------------------------------------------

ConnectionTask::ConnectionTask() 
  :task_name(0)
  ,task_id(0)
{
}

// -----------------------------------------------------------

std::string remoxly_websocket_reason_to_string(enum libwebsocket_callback_reasons reason) {
  switch(reason) {
    case LWS_CALLBACK_ESTABLISHED: return "LWS_CALLBACK_ESTABLISHED"; 
    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR: return "LWS_CALLBACK_CLIENT_CONNECTION_ERROR"; 
    case LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH: return "LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH"; 
    case LWS_CALLBACK_CLIENT_ESTABLISHED: return "LWS_CALLBACK_CLIENT_ESTABLISHED"; 
    case LWS_CALLBACK_CLOSED: return "LWS_CALLBACK_CLOSED"; 
    case LWS_CALLBACK_CLOSED_HTTP: return "LWS_CALLBACK_CLOSED_HTTP"; 
    case LWS_CALLBACK_RECEIVE: return "LWS_CALLBACK_RECEIVE"; 
    case LWS_CALLBACK_CLIENT_RECEIVE: return "LWS_CALLBACK_CLIENT_RECEIVE"; 
    case LWS_CALLBACK_CLIENT_RECEIVE_PONG: return "LWS_CALLBACK_CLIENT_RECEIVE_PONG"; 
    case LWS_CALLBACK_CLIENT_WRITEABLE: return "LWS_CALLBACK_CLIENT_WRITEABLE"; 
    case LWS_CALLBACK_SERVER_WRITEABLE: return "LWS_CALLBACK_SERVER_WRITEABLE"; 
    case LWS_CALLBACK_HTTP: return "LWS_CALLBACK_HTTP"; 
    case LWS_CALLBACK_HTTP_FILE_COMPLETION: return "LWS_CALLBACK_HTTP_FILE_COMPLETION"; 
    case LWS_CALLBACK_HTTP_WRITEABLE: return "LWS_CALLBACK_HTTP_WRITEABLE"; 
    case LWS_CALLBACK_FILTER_NETWORK_CONNECTION: return "LWS_CALLBACK_FILTER_NETWORK_CONNECTION"; 
    case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION: return "LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION"; 
    case LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS: return "LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS"; 
    case LWS_CALLBACK_OPENSSL_LOAD_EXTRA_SERVER_VERIFY_CERTS: return "LWS_CALLBACK_OPENSSL_LOAD_EXTRA_SERVER_VERIFY_CERTS"; 
    case LWS_CALLBACK_OPENSSL_PERFORM_CLIENT_CERT_VERIFICATION: return "LWS_CALLBACK_OPENSSL_PERFORM_CLIENT_CERT_VERIFICATION"; 
    case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER: return "LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER"; 
    case LWS_CALLBACK_CONFIRM_EXTENSION_OKAY: return "LWS_CALLBACK_CONFIRM_EXTENSION_OKAY"; 
    case LWS_CALLBACK_CLIENT_CONFIRM_EXTENSION_SUPPORTED: return "LWS_CALLBACK_CLIENT_CONFIRM_EXTENSION_SUPPORTED"; 
    case LWS_CALLBACK_PROTOCOL_INIT: return "LWS_CALLBACK_PROTOCOL_INIT"; 
    case LWS_CALLBACK_PROTOCOL_DESTROY: return "LWS_CALLBACK_PROTOCOL_DESTROY"; 
    case LWS_CALLBACK_ADD_POLL_FD: return "LWS_CALLBACK_ADD_POLL_FD"; 
    case LWS_CALLBACK_DEL_POLL_FD: return "LWS_CALLBACK_DEL_POLL_FD"; 
    case LWS_CALLBACK_SET_MODE_POLL_FD: return "LWS_CALLBACK_SET_MODE_POLL_FD"; 
    case LWS_CALLBACK_CLEAR_MODE_POLL_FD: return "LWS_CALLBACK_CLEAR_MODE_POLL_FD"; 
    default: return "UNKNOWN REASON"; 
  }
}

} // namespace rx 
