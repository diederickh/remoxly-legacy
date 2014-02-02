cmake_minimum_required(VERSION 2.8)

project(fontbaker)

include(${CMAKE_CURRENT_LIST_DIR}/Triplet.cmake)

set(app "fontbaker")
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

if(APPLE)

  find_library(fr_corefoundation CoreFoundation)
  find_library(fr_cocoa Cocoa)
  find_library(fr_opengl OpenGL)
  find_library(fr_iokit IOKit)
  find_library(fr_corevideo CoreVideo)

  set(app_libs
    ${extern_lib_dir}/libpng.a
    ${fr_opengl}
    -lz
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

add_executable(${app} ${app_sources})

target_link_libraries(${app} ${app_libs})

install(TARGETS ${app} DESTINATION bin)



