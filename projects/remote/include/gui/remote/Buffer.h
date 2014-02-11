/*

  Buffer
  ------

  Wrapper for communication with websockets; it makes sure that we 
  keep enough bytes around the data for the PRE and POST padding.
  
  Buffer.size()    - returns the number of added bytes
  Buffer.clear()   - clears the buffer

 */
#ifndef REMOXLY_GUI_REMOTE_BUFFER_H
#define REMOXLY_GUI_REMOTE_BUFFER_H

#include <vector>
#include <string>
#include <algorithm>

extern "C" {
#  include <libwebsockets.h>
}

// -----------------------------------------------------------

class Buffer {

 public:
  Buffer();

  void set(std::string& str);              /* set the data that we contain to the given string; we will clear all other data and make sure the given data is correctly stored in the buffer (with regard to the pre/post paddings) */
  void clear();
  void reserve(size_t nbytes);             /* this will make sure that the data member can hold nbytes + the websocket pre and post padding */
  void push_back(char c); 
  size_t getTotalNumBytes();               /* returns the number of bytes that we added (e.g. using set). the returned value includes the pre/post paddings */
  size_t getDataNumBytes();                /* returns the number of bytes in the payload, w/o the pre/post paddings */
  unsigned char* ptr();

  std::vector<char>::iterator begin();
  std::vector<char>::iterator end();

 public:
  std::vector<char> data;
  size_t nbytes_added;  /* the total number of added bytes, by e.g. set(), is reset by clear, with the pre/post paddings */
};

// -----------------------------------------------------------

inline Buffer::Buffer():nbytes_added(0) {
} 

inline void Buffer::set(std::string& str) {

  reserve(str.size());  

  std::copy(str.begin(), str.end(), data.begin());
  
  nbytes_added = str.size() + LWS_SEND_BUFFER_PRE_PADDING + LWS_SEND_BUFFER_POST_PADDING ;  
}

inline std::vector<char>::iterator Buffer::begin() {
  return data.begin();
}

inline std::vector<char>::iterator Buffer::end() {
  return data.end();
}

inline void Buffer::clear() {
  nbytes_added = 0;
  data.clear();
}

inline void Buffer::push_back(char c) {
  data.push_back(c);
}

inline size_t Buffer::getTotalNumBytes() {
  return nbytes_added;
}

inline size_t Buffer::getDataNumBytes() {

  if(nbytes_added <= (LWS_SEND_BUFFER_PRE_PADDING + LWS_SEND_BUFFER_POST_PADDING)) {
    return 0;
  }

  return nbytes_added - LWS_SEND_BUFFER_PRE_PADDING - LWS_SEND_BUFFER_POST_PADDING;
}

inline unsigned char* Buffer::ptr() {
  return (unsigned char*)&data[0];
}

inline void Buffer::reserve(size_t nbytes) {
  nbytes += LWS_SEND_BUFFER_PRE_PADDING;
  nbytes += LWS_SEND_BUFFER_POST_PADDING;
  data.reserve(nbytes);
}

#endif
