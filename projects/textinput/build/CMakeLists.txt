cmake_minimum_required(VERSION 2.8)

project(textinput)

include(${CMAKE_CURRENT_LIST_DIR}/../../../shared/cmake/Remoxly.cmake)

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

set(remoxly_textinput_headers ${app_headers})

add_executable(${app} ${app_sources} ${remoxly_app_sources})

target_link_libraries(${app} ${app_libs} ${remoxly_app_libs} ${remoxly_extern_libs})

install(TARGETS ${app} DESTINATION bin)

install(FILES ${app_headers} DESTINATION include/textinput/)




