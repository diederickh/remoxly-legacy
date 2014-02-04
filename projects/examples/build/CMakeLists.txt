cmake_minimum_required(VERSION 2.8)

project(examples)

include(${CMAKE_CURRENT_LIST_DIR}/Triplet.cmake)

set(bd ${CMAKE_CURRENT_LIST_DIR}/../)
set(sd ${bd}/src/)
set(bitmapfont_bd ${bd}/../bitmapfont/)
set(textinput_bd ${bd}/../textinput/)
set(gui_bd ${bd}/../gui/)

include(${gui_bd}/build/CMakeLists.txt)

include_directories(
  ${bd}/include/
  ${extern_include_dir}
  ${bd}/../../shared/tinylib/src/
  ${bd}/../bitmapfont/include/
  ${bd}/../textinput/include/
)

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

elseif(UNIX)

  list(APPEND app_sources
    ${extern_source_dir}/GLXW/glxw.c
    )
  
  set(app_libs
    ${extern_lib_dir}/libglfw3.a
    ${extern_lib_dir}/libpng16.a
    ${extern_lib_dir}/libz.a
    GL
    X11
    Xxf86vm
    Xrandr
    pthread
    dl
    Xi
    )

endif()

if(WIN32)

  list(APPEND app_sources
    ${extern_source_dir}/GLXW/glxw.c
    )

  add_definitions(
    -DWIN32_LEAN_AND_MEAN
    )

  set(app_libs
    ${extern_lib_dir}/glfw3.lib
    Opengl32.lib
    ws2_32.lib
    psapi.lib
    iphlpapi.lib
    )

  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND app_libs  
      ${extern_lib_dir}/libpng16_staticd.lib
      ${extern_lib_dir}/zlibstaticd.lib
      )
  else()
    list(APPEND app_libs  
      ${extern_lib_dir}/libpng16_static.lib
      ${extern_lib_dir}/zlibstatic.lib
      )
  endif()

endif()

# minimal GLFW example
add_executable(glfw_minimal ${sd}/glfw_minimal.cpp) 
target_link_libraries(glfw_minimal ${app_libs} remoxly)

install(TARGETS glfw_minimal DESTINATION bin)





