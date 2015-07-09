#!/bin/bash

build_m4=n
build_autoconf=n        # needs an updated m4 
build_libtool=n
build_automake=n        # needs autoconf
build_pkgconfig=n
build_gtkdoc=n         
build_pixman=n
build_gettext=n
build_libxml=n
build_ffi=n              
build_fontconfig=1      # needs freetype, libxml
build_libpng=y
build_libjpg=n
build_colm=n
build_ragel=n           # needs colm
build_harfbuzz=n        # needs ragel
build_freetype=n
build_glib=n            # needs ffi 
build_cairo=n           # needs png, freetype, harfbuzz 
build_pango=n           # needs glib, ffi, gtkdoc, fontconfig, freetype, libxml, harfbuzz
build_libz=y
build_yasm=n
build_libuv=n
build_mongoose=n
build_netskeleton=n
build_sslwrapper=n
build_rapidxml=y
build_glad=y
build_glfw=y
build_tinylib=y
build_videocapture=n
build_imagemagick=n
build_graphicsmagick=n
build_libav=n
build_microprofile=n
build_ogg=n
build_theora=n
build_vorbis=n
build_rxpplayer=n       # ogg,theora,vorbis player
build_tracker=n         # openGL/openCV based video tracking
build_opencv=n
build_curl=n
build_jansson=n
build_x264=n            # needs yasm 
build_flvmeta=n         
build_videogenerator=n
build_nasm=n           
build_lame=n            # needs nasm, mp3 encoding
build_portaudio=n
build_libyuv=n           
build_nanovg=n
build_liblo=n           # needs autotools/make, OSC implementation.
build_remoxly=n         # needs tinylib
build_h264bitstream=n   # h264 bitstream parser
build_openssl=n
build_httpparser=n
build_dxt5=n
build_screencapture=n
build_crypt=n           # openwall crypt library
build_sdl1=n
build_sdl2=n
