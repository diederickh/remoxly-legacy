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

# Update 2014.10.08 - we're using the build_unix_dependencies.sh
if (FALSE)
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
endif()

if(APPLE)

  find_library(fr_corefoundation CoreFoundation)
  find_library(fr_cocoa Cocoa)
  find_library(fr_opengl OpenGL)
  find_library(fr_iokit IOKit)
  find_library(fr_corevideo CoreVideo)

  set(remoxly_app_libs
    ${fr_corefoundation}
    ${fr_cocoa}
    ${fr_opengl}
    ${fr_iokit}  
    ${fr_corevideo}
    -lz
    )

  set(remoxly_extern_libs
    ${EXTERN_LIB_DIR}/libglfw3.a
    ${EXTERN_LIB_DIR}/libpng.a
#      ${EXTERN_LIB_DIR}/libjansson.a
#      ${EXTERN_LIB_DIR}/libwebsockets.a
#      ${EXTERN_LIB_DIR}/libssl.a
#      ${EXTERN_LIB_DIR}/libcrypto.a
     )

elseif(UNIX)

  # 2014.10.11, using glad now.
#  list(APPEND app_sources
#    ${EXTERN_SRC_DIR}/GLXW/glxw.c
#    )

#  list(APPEND app_sources
#    ${EXTERN_SRC_DIR}/glad.c
#    )
  
  set(remoxly_app_libs
    ${EXTERN_LIB_DIR}/libglfw3.a
    ${EXTERN_LIB_DIR}/libpng.a
    ${EXTERN_LIB_DIR}/libz.a
    pthread
    dl
    rt
    GL
    X11
    Xxf86vm
    Xrandr
    dl
    Xi
    Xcursor
    Xinerama
    udev                                   # libvideocapture
    asound                                 # audio capture
    )

elseif(WIN32)

  # 2014.10.11, using glad now.
  #list(APPEND remoxly_app_sources
  #  ${EXTERN_SRC_DIR}/GLXW/glxw.c
  #  )

  add_definitions(
    -DWIN32_LEAN_AND_MEAN
    )

  set(remoxly_app_libs
    Opengl32.lib
    ws2_32.lib
    psapi.lib
    iphlpapi.lib
    )

  list(APPEND remoxly_extern_libs
    ${EXTERN_LIB_DIR}/glfw3.lib
# 2014.10.11, we disabled remoting (not sure if we're going to use websockets or osc)
#    ${EXTERN_LIB_DIR}/libeay32.lib
#    ${EXTERN_LIB_DIR}/ssleay32.lib
#    ${EXTERN_LIB_DIR}/websockets_static.lib
    )

  if(CMAKE_BUILD_TYPE STREQUAL "Debug")

    list(APPEND remoxly_extern_libs
      ${EXTERN_LIB_DIR}/libpng12_staticd.lib
      ${EXTERN_LIB_DIR}/zlibstaticd.lib
      ${EXTERN_LIB_DIR}/jansson_d.lib
      )
  else()

    list(APPEND remoxly_extern_libs
      ${EXTERN_LIB_DIR}/libpng16_static.lib
      ${EXTERN_LIB_DIR}/zlibstatic.lib
#      ${EXTERN_LIB_DIR}/jansson.lib
      )

  endif()

endif()

set(remoxly_remote_extern_headers
  ${EXTERN_INC_DIR}/jansson_config.h
  ${EXTERN_INC_DIR}/jansson.h
  ${EXTERN_INC_DIR}/libwebsockets.h
  ${EXTERN_INC_DIR}/rapidxml_iterators.hpp
  ${EXTERN_INC_DIR}/rapidxml_print.hpp
  ${EXTERN_INC_DIR}/rapidxml_utils.hpp
  ${EXTERN_INC_DIR}/rapidxml.hpp    
  ${EXTERN_INC_DIR}/zconf.h
  ${EXTERN_INC_DIR}/zlib.h  
  )

set(remoxly_remote_extern_ssl_headers
  ${EXTERN_INC_DIR}/openssl
  )

if(WIN32)
  list(APPEND remoxly_remote_extern_headers
    ${EXTERN_INC_DIR}/gettimeofday.h
    ${EXTERN_INC_DIR}/websock-w32.h
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
#     ${EXTERN_LIB_DIR}/libglfw3.a
#     ${EXTERN_LIB_DIR}/libpng.a
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
#     ${EXTERN_SRC_DIR}/GLXW/glxw.c
#     )
#   
#   set(app_libs
#     ${EXTERN_LIB_DIR}/libglfw3.a
#     ${EXTERN_LIB_DIR}/libpng16.a
#     ${EXTERN_LIB_DIR}/libz.a
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
#    ${EXTERN_LIB_DIR}/libglfw3.a
#    ${EXTERN_LIB_DIR}/libwebsockets.a
#    ${EXTERN_LIB_DIR}/libssl.a
#    ${EXTERN_LIB_DIR}/libcrypto.a
#    ${EXTERN_LIB_DIR}/libjansson.a
#    ${EXTERN_LIB_DIR}/libuv.a
#    ${EXTERN_LIB_DIR}/libpng.a
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
#    ${EXTERN_SRC_DIR}/GLXW/glxw.c
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
#    ${EXTERN_LIB_DIR}/glfw3.lib
#    ${EXTERN_LIB_DIR}/libeay32.lib
#    ${EXTERN_LIB_DIR}/ssleay32.lib
#    ${EXTERN_LIB_DIR}/jansson.lib
#    ${EXTERN_LIB_DIR}/websockets_static.lib
#    Opengl32.lib
#    ws2_32.lib
#    psapi.lib
#    iphlpapi.lib
#    )
#
#  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#    list(APPEND app_libs  
#      ${EXTERN_LIB_DIR}/libpng16_staticd.lib
#      ${EXTERN_LIB_DIR}/zlibstaticd.lib
#      )
#  else()
#    list(APPEND app_libs  
#      ${EXTERN_LIB_DIR}/libpng16_static.lib
#      ${EXTERN_LIB_DIR}/zlibstatic.lib
#      )
#  endif()
#
#endif()

# if(APPLE) 
# 
#   # Used when packing the OF addon 
#   set(remoxly_remote_extern_libs
#     ${EXTERN_LIB_DIR}/libjansson.a
#     ${EXTERN_LIB_DIR}/libwebsockets.a
#     ${EXTERN_LIB_DIR}/libssl.a
#     ${EXTERN_LIB_DIR}/libcrypto.a
#     )
# 
#   set(remoxly_remote_extern_headers
#     ${EXTERN_INC_DIR}/jansson_config.h
#     ${EXTERN_INC_DIR}/jansson.h
#     ${EXTERN_INC_DIR}/libwebsockets.h
#     ${EXTERN_INC_DIR}/rapidxml_iterators.hpp
#     ${EXTERN_INC_DIR}/rapidxml_print.hpp
#     ${EXTERN_INC_DIR}/rapidxml_utils.hpp
#     ${EXTERN_INC_DIR}/rapidxml.hpp
#     )
# 
#   set(remoxly_remote_extern_ssl_headers
#     ${EXTERN_INC_DIR}/openssl
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
#     ${EXTERN_LIB_DIR}/libglfw3.a
#     ${EXTERN_LIB_DIR}/libpng.a
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
#     ${EXTERN_SRC_DIR}/GLXW/glxw.c
#     )
#   
#   set(app_libs
#     ${EXTERN_LIB_DIR}/libglfw3.a
#     ${EXTERN_LIB_DIR}/libpng16.a
#     ${EXTERN_LIB_DIR}/libz.a
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
#     ${EXTERN_SRC_DIR}/GLXW/glxw.c
#     )
# 
#   add_definitions(
#     -DWIN32_LEAN_AND_MEAN
#     )
# 
#   set(app_libs
#     ${EXTERN_LIB_DIR}/glfw3.lib
# 
#     Opengl32.lib
#     ws2_32.lib
#     psapi.lib
#     iphlpapi.lib
#     )
# 
#   if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#     list(APPEND app_libs  
#       ${EXTERN_LIB_DIR}/libpng16_staticd.lib
#       ${EXTERN_LIB_DIR}/zlibstaticd.lib
#       )
#   else()
#     list(APPEND app_libs  
#       ${EXTERN_LIB_DIR}/libpng16_static.lib
#       ${EXTERN_LIB_DIR}/zlibstatic.lib
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
#     ${EXTERN_LIB_DIR}/libglfw3.a
#     ${EXTERN_LIB_DIR}/libpng.a
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
#     ${EXTERN_SRC_DIR}/GLXW/glxw.c
#     )
#   
#   set(app_libs
#     ${EXTERN_LIB_DIR}/libglfw3.a
#     ${EXTERN_LIB_DIR}/libpng16.a
#     ${EXTERN_LIB_DIR}/libz.a
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
#     ${EXTERN_SRC_DIR}/GLXW/glxw.c
#     )
# 
#   add_definitions(
#     -DWIN32_LEAN_AND_MEAN
#     )
# 
#   set(app_libs
#     ${EXTERN_LIB_DIR}/glfw3.lib
#     Opengl32.lib
#     ws2_32.lib
#     psapi.lib
#     iphlpapi.lib
#     )
# 
#   if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#     list(APPEND app_libs  
#       ${EXTERN_LIB_DIR}/libpng16_staticd.lib
#       ${EXTERN_LIB_DIR}/zlibstaticd.lib
#       )
#   else()
#     list(APPEND app_libs  
#       ${EXTERN_LIB_DIR}/libpng16_static.lib
#       ${EXTERN_LIB_DIR}/zlibstatic.lib
#       )
#   endif()
# 
# endif()
