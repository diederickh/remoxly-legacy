cmake_minimum_required(VERSION 2.8)

include(${CMAKE_CURRENT_LIST_DIR}/Triplet.cmake)

set(app "textinput")
set(bd ${CMAKE_CURRENT_LIST_DIR}/../)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set(app "${app}_debug")
endif()

include_directories(
  ${bd}/include/
  ${extern_include_dir}
  ${bd}/../../shared/tinylib/src/
  ${bd}/../bitmapfont/include/
)

set(app_sources
  ${bd}/src/main.cpp
)

set(app_headers
  ${bd}/include/textinput/TextInput.h
)

add_executable(${app} ${app_sources})

if(APPLE)

  find_library(fr_corefoundation CoreFoundation)
  find_library(fr_cocoa Cocoa)
  find_library(fr_opengl OpenGL)
  find_library(fr_iokit IOKit)
  find_library(fr_corevideo CoreVideo)

  set(app_libs
    ${extern_lib_dir}/libglfw3.a
    ${extern_lib_dir}/libpng.a
    ${fr_corefoundation}
    ${fr_cocoa}
    ${fr_opengl}
    ${fr_iokit}  
    ${fr_corevideo}
    -lz
    )
endif()

target_link_libraries(${app} ${app_libs})

install(TARGETS ${app} DESTINATION bin)
install(FILES ${app_headers} DESTINATION include/textinput/)




