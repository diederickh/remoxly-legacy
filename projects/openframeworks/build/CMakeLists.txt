cmake_minimum_required(VERSION 2.8)

include(${CMAKE_CURRENT_LIST_DIR}/../../../shared/cmake/Remoxly.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../remote/build/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/../../gui/build/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/../../bitmapfont/build/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/../../textinput/build/CMakeLists.txt)

set(bd ${CMAKE_CURRENT_LIST_DIR}/..)

set(ofxremoxly_sources 
  ${bd}/src/ofxRemoxly/ofxRemoxly.cpp
)

set(ofxremoxly_headers
  ${bd}/include/ofxRemoxly/ofxRemoxly.h
)

install(FILES ${remoxly_lib_gui_sources} DESTINATION thirdparty/ofxRemoxly/src/gui )
install(FILES ${remoxly_lib_gui_headers} DESTINATION thirdparty/ofxRemoxly/include/gui )
install(FILES ${remoxly_lib_storage_headers} DESTINATION thirdparty/ofxRemoxly/include/gui/storage/ )
install(FILES ${remoxly_lib_gl_headers} DESTINATION thirdparty/ofxRemoxly/include/gui/gl/ )
install(FILES ${remoxly_lib_font_headers} DESTINATION thirdparty/ofxRemoxly/include/gui/fonts/ )
install(FILES ${remoxly_lib_storage_sources} DESTINATION thirdparty/ofxRemoxly/src/gui/storage/ )
install(FILES ${remoxly_remote_sources} DESTINATION thirdparty/ofxRemoxly/src/gui/remote/)
install(FILES ${remoxly_remote_headers} DESTINATION thirdparty/ofxRemoxly/include/gui/remote/)
install(FILES ${remoxly_remote_extern_headers} DESTINATION thirdparty/ofxRemoxly/include/)
install(FILES ${remoxly_bitmapfont_headers} DESTINATION thirdparty/ofxRemoxly/include/bitmapfont/)
install(FILES ${remoxly_textinput_headers} DESTINATION thirdparty/ofxRemoxly/include/textinput/)
install(DIRECTORY ${remoxly_remote_extern_ssl_headers} DESTINATION thirdparty/ofxRemoxly/include/)
install(FILES ${remoxly_extern_libs} DESTINATION thirdparty/ofxRemoxly/libs/)
install(FILES ${ofxremoxly_sources} DESTINATION thirdparty/ofxRemoxly/src/)
install(FILES ${ofxremoxly_headers} DESTINATION thirdparty/ofxRemoxly/include/ofxRemoxly/)

