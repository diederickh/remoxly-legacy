cmake_minimum_required(VERSION 2.8)

include(${CMAKE_CURRENT_LIST_DIR}/Triplet.cmake)

# Use remoxly_download_data_file(texture.png) to download a file to the 
# data directory. We use this feature of CMake so we don't have to store
# binaries in our repository.
set(remoxly_download_http "http://remoxly.roxlu.com/downloads/binaries/")
set(remoxly_base_dir ${CMAKE_CURRENT_LIST_DIR}/../../)

set(bitmapfont_bd ${remoxly_base_dir}/projects/bitmapfont/)
set(textinput_bd ${remoxly_base_dir}/projects/textinput/)
set(gui_bd ${remoxly_base_dir}/projects/gui/)
set(remote_bd ${remoxly_base_dir}/projects/remote/)
set(examples_bd ${remoxly_base_dir}/projects/examples/)

macro(remoxly_download_data_file file )
  if(NOT EXISTS ${install_dir}/bin/data/${file}) 
    message(STATUS "Downloading binary file ${file}")
    file(DOWNLOAD ${remoxly_download_http}${file} ${install_dir}/bin/data/${file} SHOW_PROGRESS TIMEOUT 5)
  endif()
endmacro()

if(NOT EXISTS ${remoxly_base_dir}/extern)

  execute_process(COMMAND 
    ${CMAKE_COMMAND} -E tar xzf ${remoxly_base_dir}/extern.tar.gz
    WORKING_DIRECTORY ${remoxly_base_dir}
    )

endif()

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

  set(remoxly_libs ${app_libs})

  set(remoxly_extern_libs
     ${extern_lib_dir}/libjansson.a
     ${extern_lib_dir}/libwebsockets.a
     ${extern_lib_dir}/libssl.a
     ${extern_lib_dir}/libcrypto.a
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

elseif(WIN32)

  list(APPEND remoxly_app_sources
    ${extern_source_dir}/GLXW/glxw.c
    )

  add_definitions(
    -DWIN32_LEAN_AND_MEAN
    )

  set(remoxly_app_libs
    ${extern_lib_dir}/glfw3.lib
    ${extern_lib_dir}/libeay32.lib
    ${extern_lib_dir}/ssleay32.lib
    ${extern_lib_dir}/websockets_static.lib
    Opengl32.lib
    ws2_32.lib
    psapi.lib
    iphlpapi.lib
    )

  list(APPEND remoxly_extern_libs
    ${extern_lib_dir}/glfw3.lib
    ${extern_lib_dir}/libeay32.lib
    ${extern_lib_dir}/ssleay32.lib
    ${extern_lib_dir}/websockets_static.lib
    )

  if(CMAKE_BUILD_TYPE STREQUAL "Debug")

    list(APPEND remoxly_app_libs  
      ${extern_lib_dir}/libpng16_staticd.lib
      ${extern_lib_dir}/zlibstaticd.lib
      ${extern_lib_dir}/jansson_d.lib
      )

    list(APPEND remoxly_extern_libs
      ${extern_lib_dir}/libpng16_staticd.lib
      ${extern_lib_dir}/zlibstaticd.lib
      ${extern_lib_dir}/jansson_d.lib
      )
  else()

    list(APPEND remoxly_extern_libs
      ${extern_lib_dir}/libpng16_static.lib
      ${extern_lib_dir}/zlibstatic.lib
      ${extern_lib_dir}/jansson.lib
      )

    list(APPEND remoxly_app_libs  
      ${extern_lib_dir}/libpng16_static.lib
      ${extern_lib_dir}/zlibstatic.lib
      ${extern_lib_dir}/jansson.lib
      )
  endif()

endif()

set(remoxly_remote_extern_headers
  ${extern_include_dir}/jansson_config.h
  ${extern_include_dir}/jansson.h
  ${extern_include_dir}/libwebsockets.h
  ${extern_include_dir}/rapidxml_iterators.hpp
  ${extern_include_dir}/rapidxml_print.hpp
  ${extern_include_dir}/rapidxml_utils.hpp
  ${extern_include_dir}/rapidxml.hpp    
  ${extern_include_dir}/zconf.h
  ${extern_include_dir}/zlib.h  
  )

set(remoxly_remote_extern_ssl_headers
  ${extern_include_dir}/openssl
  )

if(WIN32)
  list(APPEND remoxly_remote_extern_headers
    ${extern_include_dir}/gettimeofday.h
    ${extern_include_dir}/websock-w32.h
    )
endif()

# if(APPLE)
# 
#   find_library(fr_corefoundation CoreFoundation)
#   find_library(fr_cocoa Cocoa)
#   find_library(fr_opengl OpenGL)
#   find_library(fr_iokit IOKit)
#   find_library(fr_corevideo CoreVideo)
# 
#   set(app_libs
#     ${extern_lib_dir}/libglfw3.a
#     ${extern_lib_dir}/libpng.a
#     ${fr_corefoundation}
#     ${fr_cocoa}
#     ${fr_opengl}
#     ${fr_iokit}  
#     ${fr_corevideo}
#     -lz
#     )
# 
# elseif(UNIX)
# 
#   list(APPEND app_sources
#     ${extern_source_dir}/GLXW/glxw.c
#     )
#   
#   set(app_libs
#     ${extern_lib_dir}/libglfw3.a
#     ${extern_lib_dir}/libpng16.a
#     ${extern_lib_dir}/libz.a
#     GL
#     X11
#     Xxf86vm
#     Xrandr
#     pthread
#     dl
#     Xi
#     )
# 
# endif()
#if(APPLE)
#
#  find_library(fr_foundation CoreFoundation)
#  find_library(fr_cs CoreServices)
#  find_library(fr_opengl OpenGL)
#  find_library(fr_cocoa Cocoa)
#  find_library(fr_iokit IOKit)
#  find_library(fr_corevideo CoreVideo)
#
#  set(app_libs
#    ${extern_lib_dir}/libglfw3.a
#    ${extern_lib_dir}/libwebsockets.a
#    ${extern_lib_dir}/libssl.a
#    ${extern_lib_dir}/libcrypto.a
#    ${extern_lib_dir}/libjansson.a
#    ${extern_lib_dir}/libuv.a
#    ${extern_lib_dir}/libpng.a
#    ${fr_foundation}
#    ${fr_cs}
#    ${fr_opengl}
#    ${fr_iokit}  
#    ${fr_corevideo}
#    ${fr_cocoa}
#    -lz
#    )
#
#  set(remoxly_remote_libs ${app_libs})
#
#elseif(UNIX)
#
#endif()
#
#if(WIN32)
#
#  list(APPEND app_sources
#    ${extern_source_dir}/GLXW/glxw.c
#    )
#
#  add_definitions(
#    -DWIN32_LEAN_AND_MEAN
#    )
#
## Disable some warnings
##   "/D_CRT_SECURE_NO_WARNINGS"
##   "/wd4005"
##   "/nologo"
##   "/wd4244"
##   "/wd4267"
##   "/wd4305"
#
#  set(app_libs
#    ${extern_lib_dir}/glfw3.lib
#    ${extern_lib_dir}/libeay32.lib
#    ${extern_lib_dir}/ssleay32.lib
#    ${extern_lib_dir}/jansson.lib
#    ${extern_lib_dir}/websockets_static.lib
#    Opengl32.lib
#    ws2_32.lib
#    psapi.lib
#    iphlpapi.lib
#    )
#
#  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#    list(APPEND app_libs  
#      ${extern_lib_dir}/libpng16_staticd.lib
#      ${extern_lib_dir}/zlibstaticd.lib
#      )
#  else()
#    list(APPEND app_libs  
#      ${extern_lib_dir}/libpng16_static.lib
#      ${extern_lib_dir}/zlibstatic.lib
#      )
#  endif()
#
#endif()

# if(APPLE) 
# 
#   # Used when packing the OF addon 
#   set(remoxly_remote_extern_libs
#     ${extern_lib_dir}/libjansson.a
#     ${extern_lib_dir}/libwebsockets.a
#     ${extern_lib_dir}/libssl.a
#     ${extern_lib_dir}/libcrypto.a
#     )
# 
#   set(remoxly_remote_extern_headers
#     ${extern_include_dir}/jansson_config.h
#     ${extern_include_dir}/jansson.h
#     ${extern_include_dir}/libwebsockets.h
#     ${extern_include_dir}/rapidxml_iterators.hpp
#     ${extern_include_dir}/rapidxml_print.hpp
#     ${extern_include_dir}/rapidxml_utils.hpp
#     ${extern_include_dir}/rapidxml.hpp
#     )
# 
#   set(remoxly_remote_extern_ssl_headers
#     ${extern_include_dir}/openssl
#     )
# 
# endif()

# if(APPLE)
# 
#   find_library(fr_corefoundation CoreFoundation)
#   find_library(fr_cocoa Cocoa)
#   find_library(fr_opengl OpenGL)
#   find_library(fr_iokit IOKit)
#   find_library(fr_corevideo CoreVideo)
# 
#   set(app_libs
#     ${extern_lib_dir}/libglfw3.a
#     ${extern_lib_dir}/libpng.a
#     ${fr_corefoundation}
#     ${fr_cocoa}
#     ${fr_opengl}
#     ${fr_iokit}  
#     ${fr_corevideo}
#     -lz
#     )
# 
# elseif(UNIX)
# 
#   list(APPEND app_sources
#     ${extern_source_dir}/GLXW/glxw.c
#     )
#   
#   set(app_libs
#     ${extern_lib_dir}/libglfw3.a
#     ${extern_lib_dir}/libpng16.a
#     ${extern_lib_dir}/libz.a
#     GL
#     X11
#     Xxf86vm
#     Xrandr
#     pthread
#     dl
#     Xi
#     )
# 
# endif()

# if(WIN32)
# 
#   list(APPEND app_sources
#     ${extern_source_dir}/GLXW/glxw.c
#     )
# 
#   add_definitions(
#     -DWIN32_LEAN_AND_MEAN
#     )
# 
#   set(app_libs
#     ${extern_lib_dir}/glfw3.lib
# 
#     Opengl32.lib
#     ws2_32.lib
#     psapi.lib
#     iphlpapi.lib
#     )
# 
#   if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#     list(APPEND app_libs  
#       ${extern_lib_dir}/libpng16_staticd.lib
#       ${extern_lib_dir}/zlibstaticd.lib
#       )
#   else()
#     list(APPEND app_libs  
#       ${extern_lib_dir}/libpng16_static.lib
#       ${extern_lib_dir}/zlibstatic.lib
#       )
#   endif()
# 
# endif()


# if(APPLE)
# 
#   find_library(fr_corefoundation CoreFoundation)
#   find_library(fr_cocoa Cocoa)
#   find_library(fr_opengl OpenGL)
#   find_library(fr_iokit IOKit)
#   find_library(fr_corevideo CoreVideo)
# 
#   set(app_libs
#     ${extern_lib_dir}/libglfw3.a
#     ${extern_lib_dir}/libpng.a
#     ${fr_corefoundation}
#     ${fr_cocoa}
#     ${fr_opengl}
#     ${fr_iokit}  
#     ${fr_corevideo}
#     -lz
#     )
# 
# elseif(UNIX)
# 
#   list(APPEND app_sources
#     ${extern_source_dir}/GLXW/glxw.c
#     )
#   
#   set(app_libs
#     ${extern_lib_dir}/libglfw3.a
#     ${extern_lib_dir}/libpng16.a
#     ${extern_lib_dir}/libz.a
#     GL
#     X11
#     Xxf86vm
#     Xrandr
#     pthread
#     dl
#     Xi
#     )
# 
# endif()
# 
# if(WIN32)
# 
#   list(APPEND app_sources
#     ${extern_source_dir}/GLXW/glxw.c
#     )
# 
#   add_definitions(
#     -DWIN32_LEAN_AND_MEAN
#     )
# 
#   set(app_libs
#     ${extern_lib_dir}/glfw3.lib
#     Opengl32.lib
#     ws2_32.lib
#     psapi.lib
#     iphlpapi.lib
#     )
# 
#   if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#     list(APPEND app_libs  
#       ${extern_lib_dir}/libpng16_staticd.lib
#       ${extern_lib_dir}/zlibstaticd.lib
#       )
#   else()
#     list(APPEND app_libs  
#       ${extern_lib_dir}/libpng16_static.lib
#       ${extern_lib_dir}/zlibstatic.lib
#       )
#   endif()
# 
# endif()
