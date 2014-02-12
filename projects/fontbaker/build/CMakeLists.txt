cmake_minimum_required(VERSION 2.8)

project(fontbaker)

include(${CMAKE_CURRENT_LIST_DIR}/../../../shared/cmake/Remoxly.cmake)

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

add_executable(${app} ${app_sources} ${remoxly_app_sources})

target_link_libraries(${app} ${app_libs} ${remoxly_app_libs})

install(TARGETS ${app} DESTINATION bin)



