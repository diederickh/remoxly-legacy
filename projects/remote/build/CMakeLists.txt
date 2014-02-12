cmake_minimum_required(VERSION 2.8)

project(remote)

include(${CMAKE_CURRENT_LIST_DIR}/../../../shared/cmake/Remoxly.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../gui/build/CMakeLists.txt)

set(app "remote_control")

set(bd ${CMAKE_CURRENT_LIST_DIR}/../)
set(gui_bd ${bd}../gui/)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set(app "${app}_debug")
endif()

include_directories(
  ${bd}/include/
  ${bd}/../gui/include/
  ${extern_include_dir}
)

set(remoxly_remote_sources
  ${bd}/src/gui/remote/Server.cpp
  ${bd}/src/gui/remote/Serializer.cpp
  ${bd}/src/gui/remote/Deserializer.cpp
  ${bd}/src/gui/remote/Client.cpp
  ${bd}/src/gui/remote/Utils.cpp
)

set(remoxly_remote_headers
  ${bd}/include/gui/remote/Buffer.h
  ${bd}/include/gui/remote/Client.h
  ${bd}/include/gui/remote/ClientListener.h
  ${bd}/include/gui/remote/Deserializer.h
  ${bd}/include/gui/remote/Generator.h
  ${bd}/include/gui/remote/Remote.h
  ${bd}/include/gui/remote/Serializer.h
  ${bd}/include/gui/remote/Server.h
  ${bd}/include/gui/remote/Types.h
  ${bd}/include/gui/remote/Utils.h
)

set(app_sources
  ${bd}/src/main.cpp
)


if(NOT TARGET ${app})
  add_executable(${app} ${app_sources} ${remoxly_app_sources})

  add_library(remoxly_remote ${remoxly_remote_sources})

  target_link_libraries(${app} ${app_libs} remoxly remoxly_remote ${remoxly_app_libs})

  install(TARGETS ${app} DESTINATION bin)
endif()


