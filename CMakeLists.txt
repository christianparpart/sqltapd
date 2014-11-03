cmake_minimum_required(VERSION 2.8)

project(sqltap)
set(SQLTAP_NAME "sqltap")
set(SQLTAP_VERSION "0.0.0")
set(SQLTAP_HOMEPAGE_URL "http://xzero.io/")
set(SQLTAP_BUGTRACKER_URL "https://github.com/xzero/libxzero/issues/")

include(FindPkgConfig)
include(FindDoxygen)
include(CheckIncludeFiles)
include(CheckIncludeFileCXX)
include(CheckFunctionExists)
include(CheckVariableExists)
include(CheckTypeSize)
include(CheckLibraryExists)
include(CheckCSourceCompiles)
include(CMakeDetermineCCompiler)

option(ENABLE_TESTS "Build unit tests [default: off]" OFF)
option(ENABLE_EXAMPLES "Includes example plugins and programs [default: off]" OFF)

pkg_check_modules(XZERO REQUIRED "libxzero")
include_directories(${XZERO_INCLUDE_DIRS})
link_directories(${XZERO_LIBRARY_DIRS})
link_libraries(${XZERO_LIBRARIES})

pkg_check_modules(LIBXML REQUIRED "libxml-2.0")
include_directories(${LIBXML_INCLUDE_DIRS})
link_directories(${LIBXML_LIBRARY_DIRS})
link_libraries(${LIBXML_LIBRARIES})

set(CMAKE_CXX_FLAGS "-std=c++0x")                   # C++11
#set(CMAKE_CXX_FLAGS "-std=c++0x -stdlib=libc++")   # C++11 + libc++
#set(CMAKE_CXX_FLAGS "-std=c++1y -stdlib=libc++")   # C++14 + libc++
#set(CMAKE_CXX_FLAGS "-std=c++1z -stdlib=libc++")   # C++17 + libc++

CHECK_INCLUDE_FILES(zlib.h HAVE_ZLIB_H)
if(HAVE_ZLIB_H)
  CHECK_LIBRARY_EXISTS(z deflate "" HAVE_LIBZ)
  set(ZLIB_LIBRARIES z)
endif(HAVE_ZLIB_H)

CHECK_FUNCTION_EXISTS(sysconf HAVE_SYSCONF)
CHECK_FUNCTION_EXISTS(pathconf HAVE_PATHCONF)

CHECK_INCLUDE_FILES(pthread.h HAVE_PTHREAD_H)
CHECK_LIBRARY_EXISTS(pthread pthread_setname_np "" HAVE_PTHREAD_SETNAME_NP)
CHECK_LIBRARY_EXISTS(pthread pthread_setaffinity_np "" HAVE_PTHREAD_SETAFFINITY_NP)

if(ENABLE_INOTIFY)
  CHECK_INCLUDE_FILES(sys/inotify.h HAVE_SYS_INOTIFY_H)
  if(HAVE_SYS_INOTIFY_H)
    CHECK_FUNCTION_EXISTS(inotify_init1 HAVE_INOTIFY_INIT1)
  endif(HAVE_SYS_INOTIFY_H)
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if(ENABLE_TESTS)
  CHECK_INCLUDE_FILES(gtest/gtest.h HAVE_GTEST_GTEST_H)
endif()

add_definitions(-DHAVE_SYSCONFIG_H)

add_custom_target(ctags
    COMMAND ctags -R --c++-kinds=+p --fields=+iaS --extra=+q ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/include
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

add_custom_target(cscope
    COMMAND echo -n > ${CMAKE_SOURCE_DIR}/cscope.files
            && find ${CMAKE_SOURCE_DIR}/src -name '*.h' -o -name '*.cc' -o -name '*.cpp' >> ${CMAKE_SOURCE_DIR}/cscope.files
            && find ${CMAKE_SOURCE_DIR}/include -name '*.h' -o -name '*.cc' -o -name '*.hpp' >> ${CMAKE_SOURCE_DIR}/cscope.files
            && cscope -q -R -b -i cscope.files
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/sqltap/sysconfig.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/include/sqltap/sysconfig.h)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/sqltap.pc.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/sqltap.pc)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/sqltap.pc
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)

#add_subdirectory(doc)
add_subdirectory(src)
