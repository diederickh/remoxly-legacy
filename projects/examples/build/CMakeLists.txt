cmake_minimum_required(VERSION 2.8)

project(examples)

include(${CMAKE_CURRENT_LIST_DIR}/../../../shared/cmake/Remoxly.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../remote/build/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/../../gui/build/CMakeLists.txt)

remoxly_download_data_file(texture.png)

set(bd ${CMAKE_CURRENT_LIST_DIR}/../)
set(sd ${bd}/src/)

include_directories(
  ${bd}/include/
  ${extern_include_dir}
  ${bd}/../../shared/tinylib/src/
  ${bd}/../bitmapfont/include/
  ${bd}/../textinput/include/
  ${bd}/../remote/include/
)

set(debug_flag "")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(debug_flag "_debug")
endif()

# minimal GLFW example
add_executable("glfw_minimal${debug_flag}" ${remoxly_app_sources} ${sd}/glfw_minimal.cpp)
target_link_libraries("glfw_minimal${debug_flag}" ${remoxly_remote_libs} remoxly remoxly_remote  ${remoxly_app_libs} ${remoxly_extern_libs})
install(TARGETS "glfw_minimal${debug_flag}" DESTINATION bin)

# server example 
add_executable("remote_server${debug_flag}" ${sd}/remote_server.cpp ${remoxly_app_sources})
target_link_libraries("remote_server${debug_flag}" ${remoxly_libs} ${remoxly_remote_libs} remoxly remoxly_remote ${remoxly_app_libs} ${remoxly_extern_libs})
install(TARGETS "remote_server${debug_flag}" DESTINATION bin)


