# You can define the following variables on the command line:
#
#  - EXTERN_LIB_DIR         Path to the directory where we can find e.g. libpng, libz, etc..
#  - EXTERN_INC_DIR         Path to the directory where we can find general headers e.g. png.h 
#  - EXTERN_SRC_DIR         Path to the directory where we can find sources for dependencies (e.g. glad.c) 
#  - TINYLIB_DIR            Path to tinylib base dir. 
#  - REMOXLY_DONT_INSTALL   Set to `On` if you don't want to install headers and libs. If you want to edit RenderGL.h for example, you probably don't want to install the headers otherwise the compiler will use the installed version. 

cmake_minimum_required(VERSION 2.8)

project(gui)

message(STATUS "Remoxly.REMOXLY_DONT_INSTALL: ${REMOXLY_DONT_INSTALL}")

include(${CMAKE_CURRENT_LIST_DIR}/../../../shared/cmake/Remoxly.cmake)

set(app "gui")

set(bd ${CMAKE_CURRENT_LIST_DIR}/../)

# Note 2014.12.08 - was messing up a build (exxon) when including.
#list(APPEND CMAKE_C_FLAGS $ENV{CFLAGS})
#list(APPEND CMAKE_CXX_FLAGS $ENV{CFLAGS})

option(REMOXLY_DONT_INSTALL "Remoxly: do not install" OFF)        # Set to `On` if you don't want to install the headers and lib. 

if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set(app "${app}_debug")
endif()

include_directories(
  ${bd}/include/
  ${EXTERN_INC_DIR}
  ${TINYLIB_DIR}/src/
  ${bd}/../bitmapfont/include/
  ${bd}/../textinput/include/
)

set(remoxly_lib_sources
  ${bd}/src/gui/Container.cpp
  ${bd}/src/gui/Group.cpp
  ${bd}/src/gui/Utils.cpp
  ${bd}/src/gui/Widget.cpp
  ${bd}/src/gui/Render.cpp
  ${bd}/src/gui/IconButton.cpp
  ${bd}/src/gui/Scroll.cpp
  ${bd}/src/gui/Panel.cpp
  ${bd}/src/gui/Button.cpp
  ${bd}/src/gui/Toggle.cpp
  ${bd}/src/gui/Text.cpp
  ${bd}/src/gui/Texture.cpp
  ${bd}/src/gui/Select.cpp
  ${bd}/src/gui/Menu.cpp
  ${bd}/src/gui/ColorRGB.cpp
  ${bd}/src/gui/Storage.cpp
  ${bd}/src/gui/Render.cpp
  ${bd}/src/gui/storage/StorageXML.cpp
)

set(app_sources
  ${bd}/src/main.cpp
  ${remoxly_lib_sources}
)

if(NOT TARGET ${app})
  add_executable(${app} ${app_sources} ${EXTERN_SRC_DIR}/glad.c)

  add_library(remoxly STATIC ${remoxly_lib_sources})

  target_link_libraries(${app} ${remoxly_app_libs} ${remoxly_extern_libs})

  install(TARGETS ${app} DESTINATION bin)

  install(TARGETS remoxly DESTINATION lib)

  add_executable(test_app ${bd}/src/test_app.cpp ${bd}/src/App.cpp ${EXTERN_SRC_DIR}/glad.c)
  target_link_libraries(test_app ${remoxly_app_libs} ${remoxly_extern_libs} remoxly)
  install(TARGETS test_app DESTINATION bin)

  set(remoxly_lib_gui_headers
    ${bd}/include/gui/Menu.h
    ${bd}/include/gui/Select.h
    ${bd}/include/gui/Container.h
    ${bd}/include/gui/Button.h
    ${bd}/include/gui/ColorRGB.h
    ${bd}/include/gui/Group.h
    ${bd}/include/gui/IconButton.h
    ${bd}/include/gui/Panel.h
    ${bd}/include/gui/Remoxly.h
    ${bd}/include/gui/Render.h
    ${bd}/include/gui/Scroll.h
    ${bd}/include/gui/Slider.h
    ${bd}/include/gui/Storage.h
    ${bd}/include/gui/Toggle.h
    ${bd}/include/gui/Text.h
    ${bd}/include/gui/Texture.h
    ${bd}/include/gui/Types.h
    ${bd}/include/gui/Utils.h
    ${bd}/include/gui/Widget.h
    ${bd}/include/gui/WidgetListener.h
    )

  set(remoxly_lib_font_headers
    ${bd}/include/gui/fonts/FontAwesome.h
    ${bd}/include/gui/fonts/DejaVu.h
    )

  set(remoxly_lib_storage_headers
    ${bd}/include/gui/storage/StorageXML.h
    )

  set(remoxly_lib_storage_sources
    ${bd}/src/gui/storage/StorageXML.cpp
    )

  set(remoxly_lib_gl_headers
    ${bd}/include/gui/gl/RenderGL.h
    )

  set(remoxly_lib_gui_sources
    ${bd}/src/gui/Container.cpp
    ${bd}/src/gui/Group.cpp
    ${bd}/src/gui/Utils.cpp
    ${bd}/src/gui/Widget.cpp
    ${bd}/src/gui/Render.cpp
    ${bd}/src/gui/IconButton.cpp
    ${bd}/src/gui/Scroll.cpp
    ${bd}/src/gui/Panel.cpp
    ${bd}/src/gui/Button.cpp
    ${bd}/src/gui/Toggle.cpp
    ${bd}/src/gui/Text.cpp
    ${bd}/src/gui/Texture.cpp
    ${bd}/src/gui/Select.cpp
    ${bd}/src/gui/Menu.cpp
    ${bd}/src/gui/ColorRGB.cpp
    ${bd}/src/gui/Storage.cpp
    ${bd}/src/gui/Render.cpp
    )

  set(remoxly_lib_bitmapfont_headers
    ${bitmapfont_bd}/include/gui/bitmapfont/BitmapFont.h
    )

  set(remoxly_lib_textinput_headers
    ${textinput_bd}/include/gui/textinput/TextInput.h
    )

  if (NOT REMOXLY_DONT_INSTALL) 
    install(FILES ${remoxly_lib_gui_headers} DESTINATION include/gui/  )
    install(FILES ${remoxly_lib_storage_headers} DESTINATION include/gui/storage/ )
    install(FILES ${remoxly_lib_storage_sources} DESTINATION src/gui/storage/ )
    install(FILES ${remoxly_lib_gl_headers} DESTINATION include/gui/gl/ )
    install(FILES ${remoxly_lib_font_headers} DESTINATION include/gui/fonts )
    install(FILES ${remoxly_lib_gui_sources} DESTINATION src/gui )
    install(FILES ${remoxly_lib_bitmapfont_headers} DESTINATION include/gui/bitmapfont/ )
    install(FILES ${remoxly_lib_textinput_headers} DESTINATION include/gui/textinput/ )
  endif()

endif()





