cmake_minimum_required(VERSION 2.8)

# ${tri_arch}             - x86_64, i386
# ${tri_compiler}         - clang, gcc, vs2010, vs2012
# ${tri_platform}         - mac, win, unix
# ${tri_triplet}          - ${tri_platform}-${tri_compiler}-${tri_arch}
# ${install_dir}          - the install prefix
# ${extern_source_dir}    - path to the source directory inside extern/${tri_triplet}/ 
# ${extern_lib_dir}       - path to the extern libraries (root)
# ${extern_include_dir}   - include directories in for the extern libraries

if(NOT REMOXLY_USE_32BIT)
  set(tri_arch "x86_64")
else()
  set(tri_arch "i386")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(tri_compiler "clang")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(tri_compiler "gcc")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  if(MSVC10)
    set(tri_compiler "vs2010")
  else()
    set(tri_compiler "vs2012")
  endif()
endif()

if(CMAKE_GENERATOR STREQUAL "Visual Studio 11 Win64")
  set(tri_compiler "vs2012")
  set(tri_arch "x86_64")
endif()


if(APPLE)
  set(tri_platform "mac")
  set(CMAKE_OSX_ARCHITECTURES ${tri_arch})
elseif(WIN32)
  set(tri_platform "win")
else()
  set(tri_platform "linux")
endif()

set(tri_triplet "${tri_platform}-${tri_compiler}-${tri_arch}")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(tri_triplet "${tri_triplet}d")
endif()

set(install_dir ${CMAKE_CURRENT_LIST_DIR}/../../install/${tri_triplet})
set(extern_source_dir ${CMAKE_CURRENT_LIST_DIR}/../../extern/${tri_triplet}/src/)

if (${EXTERN_LIB_DIR})
  set(extern_lib_dir ${EXTERN_LIB_DIR})
else()
  set(extern_lib_dir ${CMAKE_CURRENT_LIST_DIR}/../../extern/${tri_triplet}/lib/)
endif()

if (${EXTERN_INCLUDE_DIR})
  set(extern_include_dir ${EXTERN_INCLUDE_DIR})
else()
  set(extern_include_dir ${CMAKE_CURRENT_LIST_DIR}/../../extern/${tri_triplet}/include)
endif()

# 2014.10.08, removed, the user should pass -DCMAKE_INSTALL_PREFIX to the cmake build command!
# set(CMAKE_INSTALL_PREFIX ${install_dir})
message(STATUS "Building for ${tri_triplet}")
message(STATUS "Extern include dir: ${extern_include_dir}")
