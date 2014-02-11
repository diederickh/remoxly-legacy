cmake_minimum_required(VERSION 2.8)

project(gui)

include(${CMAKE_CURRENT_LIST_DIR}/Triplet.cmake)

set(app "gui")

set(bd ${CMAKE_CURRENT_LIST_DIR}/../)
set(bitmapfont_bd ${bd}/../bitmapfont/)
set(textinput_bd ${bd}/../textinput/)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set(app "${app}_debug")
endif()

include_directories(
  ${bd}/include/
  ${extern_include_dir}
  ${bd}/../../shared/tinylib/src/
  ${bd}/../bitmapfont/include/
  ${bd}/../textinput/include/
)

set(remoxly_lib_sources
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
  ${bd}/src/gui/ColorRGB.cpp
  ${bd}/src/gui/Storage.cpp
  ${bd}/src/gui/Render.cpp
  ${bd}/src/gui/storage/StorageXML.cpp
)

set(app_sources
  ${bd}/src/main.cpp
  ${remoxly_lib_sources}
)

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


if(NOT TARGET ${app})
  add_executable(${app} ${app_sources})

  add_library(remoxly STATIC ${remoxly_lib_sources})

  target_link_libraries(${app} ${app_libs})

  install(TARGETS ${app} DESTINATION bin)

  install(TARGETS remoxly DESTINATION lib)

  set(remoxly_lib_gui_headers
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
    ${bd}/include/gui/fonts/FreePixel.h
    ${bd}/include/gui/fonts/FontAwesome.h
    ${bd}/include/gui/fonts/SourceCode.h
    ${bd}/include/gui/fonts/Arial.h
    )

  set(remoxly_lib_storage_headers
    ${bd}/include/gui/storage/StorageXML.h
    )

  set(remoxly_lib_storage_sources
    ${bd}/src/gui/storage/StorageXML.cpp
    )

  set(remoxly_lib_gl_headers
    ${bd}/include/gui/gl/RenderGL.h
    ${bd}/include/gui/gl/ImplementationGL.h
    )

  set(remoxly_lib_gui_sources
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
    ${bd}/src/gui/ColorRGB.cpp
    ${bd}/src/gui/Storage.cpp
    ${bd}/src/gui/Render.cpp
    )
  
  set(remoxly_lib_bitmapfont_headers
    ${bitmapfont_bd}/include/bitmapfont/BitmapFont.h
    )

  set(remoxly_lib_textinput_headers
    ${textinput_bd}/include/textinput/TextInput.h
    )

  install(FILES ${remoxly_lib_gui_headers} DESTINATION include/gui/  )
  install(FILES ${remoxly_lib_storage_headers} DESTINATION include/gui/storage/ )
  install(FILES ${remoxly_lib_storage_sources} DESTINATION src/gui/storage/ )
  install(FILES ${remoxly_lib_gl_headers} DESTINATION include/gui/gl/ )
  install(FILES ${remoxly_lib_font_headers} DESTINATION include/gui/gl/fonts )
  install(FILES ${remoxly_lib_gui_sources} DESTINATION src/gui )
  install(FILES ${remoxly_lib_bitmapfont_headers} DESTINATION include/bitmapfont/  )
  install(FILES ${remoxly_lib_textinput_headers} DESTINATION include/textinput/  )

endif()





