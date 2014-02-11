cmake_minimum_required(VERSION 2.8)

project(remote)

include(${CMAKE_CURRENT_LIST_DIR}/Triplet.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../gui/build/CMakeLists.txt)

set(app "remote")

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

if(APPLE) 

  # Used when packing the OF addon 
  set(remoxly_remote_extern_libs
    ${extern_lib_dir}/libjansson.a
    ${extern_lib_dir}/libwebsockets.a
    ${extern_lib_dir}/libssl.a
    ${extern_lib_dir}/libcrypto.a
    )

  set(remoxly_remote_extern_headers
    ${extern_include_dir}/jansson_config.h
    ${extern_include_dir}/jansson.h
    ${extern_include_dir}/libwebsockets.h
    ${extern_include_dir}/rapidxml_iterators.hpp
    ${extern_include_dir}/rapidxml_print.hpp
    ${extern_include_dir}/rapidxml_utils.hpp
    ${extern_include_dir}/rapidxml.hpp
    )

  set(remoxly_remote_extern_ssl_headers
    ${extern_include_dir}/openssl
    )

endif()


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

if(APPLE)

  find_library(fr_foundation CoreFoundation)
  find_library(fr_cs CoreServices)
  find_library(fr_opengl OpenGL)
  find_library(fr_cocoa Cocoa)
  find_library(fr_iokit IOKit)
  find_library(fr_corevideo CoreVideo)

  set(app_libs
    ${extern_lib_dir}/libglfw3.a
    ${extern_lib_dir}/libwebsockets.a
    ${extern_lib_dir}/libssl.a
    ${extern_lib_dir}/libcrypto.a
    ${extern_lib_dir}/libjansson.a
    ${extern_lib_dir}/libuv.a
    ${extern_lib_dir}/libpng.a
    ${fr_foundation}
    ${fr_cs}
    ${fr_opengl}
    ${fr_iokit}  
    ${fr_corevideo}
    ${fr_cocoa}
    -lz
    )

  set(remoxly_remote_libs ${app_libs})

elseif(UNIX)

endif()

if(WIN32)
endif()

if(NOT TARGET ${app})
  add_executable(${app} ${app_sources})

  add_library(remoxly_remote ${remoxly_remote_sources})

  target_link_libraries(${app} ${app_libs} remoxly remoxly_remote)

  install(TARGETS ${app} DESTINATION bin)
endif()




