# Install script for directory: /home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/base

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/hchc/projects/GitFile/jmuduo/build/release-install-cpp11")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/hchc/projects/GitFile/jmuduo/build/release-cpp11/lib/libmuduo_base.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/muduo/base" TYPE FILE FILES
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/base/Atomic.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/base/BlockingQueue.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/base/Condition.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/base/CountDownLatch.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/base/CurrentThread.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/base/Date.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/base/Exception.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/base/LogStream.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/base/Logging.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/base/Mutex.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/base/StringPiece.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/base/Thread.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/base/ThreadLocal.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/base/TimeZone.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/base/Timestamp.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/base/Types.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/base/copyable.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/base/noncopyable.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/base/tempCodeRunnerFile.h"
    )
endif()
