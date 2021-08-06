# Install script for directory: /home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/net

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/hchc/projects/GitFile/jmuduo/build/release-cpp11/lib/libmuduo_net.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/muduo/net" TYPE FILE FILES
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/net/Acceptor.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/net/Buffer.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/net/Channel.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/net/EventLoop.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/net/EventLoopThread.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/net/EventLoopThreadPool.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/net/Endian.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/net/InetAddress.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/net/TimerId.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/net/TcpClient.cc"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/net/TcpConnection.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/net/TcpServer.h"
    "/home/hchc/projects/GitFile/jmuduo/Pratice_muduo/muduo/net/TimerId.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/hchc/projects/GitFile/jmuduo/build/release-cpp11/muduo/net/http/cmake_install.cmake")
  include("/home/hchc/projects/GitFile/jmuduo/build/release-cpp11/muduo/net/inspect/cmake_install.cmake")
  include("/home/hchc/projects/GitFile/jmuduo/build/release-cpp11/muduo/net/tests/cmake_install.cmake")

endif()

