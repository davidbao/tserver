#*******************************************************************************
#  Copyright (c) 2022 com.
#
#  All rights reserved. This program and the accompanying materials
#  are made available under the terms of the EaseSuite Public License v1.0
#  and EaseSuite Distribution License v1.0 which accompany this distribution.
#
#  Contributors:
#     Baowei - initial version
#*******************************************************************************

# Note: on OS X you should install XCode and the associated command-line tools

cmake_minimum_required(VERSION 3.2.3)
project(tserver)

option(TSERVER_STATIC_STDLIB "Option for build with static libstdc++" OFF)
option(TSERVER_ENABLE_TEST "Option for build with test" ON)
option(TSERVER_BUILD_SQLITE "Option for build with sqlite" ON)
option(TSERVER_BUILD_MYSQL "Option for build with mysql" ON)
option(TSERVER_BUILD_MYSQL_STATIC "Option for build with mysql static lib" ON)
option(TSERVER_BUILD_ORACLE "Option for build with oracle" OFF)
option(TSERVER_BUILD_KINGBASE "Option for build with Kingbase" OFF)
option(TSERVER_BUILD_DM6 "Option for build with dm6" OFF)
option(TSERVER_BUILD_DM7 "Option for build with dm7" OFF)
option(TSERVER_BUILD_MQTTCLIENT "Option for build with mqtt client(use paho.mqtt.c)" ON)

## build settings
set(TSERVER_VERSION_MAJOR 1)
set(TSERVER_VERSION_MINOR 3)
set(TSERVER_VERSION_PATCH 0)
set(TSERVER_VERSION ${TSERVER_VERSION_MAJOR}.${TSERVER_VERSION_MINOR}.${TSERVER_VERSION_PATCH})

set(TSERVER_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD ${TSERVER_CXX_STANDARD})

message(STATUS "CMake version: " ${CMAKE_VERSION})
message(STATUS "CMake system name: " ${CMAKE_SYSTEM_NAME})
message(STATUS "CMake build type: " ${CMAKE_BUILD_TYPE})
message(STATUS "CMake cxx standard: " ${CMAKE_CXX_STANDARD})
message(STATUS "Tserver version: " ${TSERVER_VERSION})

set(CMAKE_VERBOSE_MAKEFILE ON)

# declare some tags.
if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    set(OS_NAME emscripten)
    set(WEB_OS ON)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(OS_NAME mac)
    set(MAC_OS ON)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux" AND ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm")
    set(OS_NAME arm)
    set(ARM_OS ON)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(OS_NAME linux)
    set(LINUX_OS ON)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    set(OS_NAME android)
    set(ANDROID_OS ON)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(OS_NAME win)
    set(WIN_OS ON)
ELSE ()
    set(OS_NAME ${CMAKE_SYSTEM_NAME})
endif ()

# show options.
message(STATUS "TSERVER_STATIC_STDLIB: " ${TSERVER_STATIC_STDLIB})
message(STATUS "TSERVER_ENABLE_TEST: " ${TSERVER_ENABLE_TEST})
message(STATUS "TSERVER_BUILD_SQLITE: " ${TSERVER_BUILD_SQLITE})
message(STATUS "TSERVER_BUILD_MYSQL:" ${TSERVER_BUILD_MYSQL})
message(STATUS "TSERVER_BUILD_MYSQL_STATIC:" ${TSERVER_BUILD_MYSQL_STATIC})
message(STATUS "TSERVER_BUILD_ORACLE:" ${TSERVER_BUILD_ORACLE})
message(STATUS "TSERVER_BUILD_KINGBASE: " ${TSERVER_BUILD_KINGBASE})
message(STATUS "TSERVER_BUILD_DM6: " ${TSERVER_BUILD_DM6})
message(STATUS "TSERVER_BUILD_DM7: " ${TSERVER_BUILD_DM7})
message(STATUS "TSERVER_BUILD_MQTTCLIENT: " ${TSERVER_BUILD_MQTTCLIENT})

# set externals path.
set(EXTERNALS_PATH ${CMAKE_SOURCE_DIR}/externals)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}-${OS_NAME})

# include files.
if (WIN_OS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCURL_STATICLIB -D_CRT_SECURE_NO_WARNINGS -D_WINSOCK_DEPRECATED_NO_WARNINGS -wd4996 -wd4244 -wd4819")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCURL_STATICLIB -D_CRT_SECURE_NO_WARNINGS -D_WINSOCK_DEPRECATED_NO_WARNINGS -wd4996 -wd4244 -wd4819")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /ignore:4099 /ignore:4286")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /ignore:4099 /ignore:4286")
    set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /ignore:4099 /ignore:4286")
endif ()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG")
else (${CMAKE_SYSTEM_NAME} STREQUAL "Release")
endif ()

# src path.
add_subdirectory(src)

# test.
if (TSERVER_ENABLE_TEST)
    enable_testing()
    add_subdirectory(tests)
endif ()
