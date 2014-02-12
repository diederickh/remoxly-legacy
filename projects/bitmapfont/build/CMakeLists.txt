cmake_minimum_required(VERSION 2.8)

project(bitmapfont)

include(${CMAKE_CURRENT_LIST_DIR}/../../../shared/cmake/Remoxly.cmake)

remoxly_download_data_file(freepixel_0.png)
remoxly_download_data_file(freepixel.fnt)

set(app "bitmapfont")

set(bd ${CMAKE_CURRENT_LIST_DIR}/../)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set(app "${app}_debug")
endif()

include_directories(
  ${bd}/include/
  ${extern_include_dir}
  ${bd}/../../shared/tinylib/src/
)

set(app_sources
  ${bd}/src/main.cpp
)

set(app_headers
  ${bitmapfont_bd}/include/bitmapfont/BitmapFont.h
)

set(remoxly_bitmapfont_headers ${app_headers})

add_executable(${app} ${app_sources} ${remoxly_app_sources})

target_link_libraries(${app} ${remoxly_app_libs})

install(TARGETS ${app} DESTINATION bin)

install(FILES ${app_headers} DESTINATION include/bitmapfont/)



