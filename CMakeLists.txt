cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED ZP1_VERSION)
  set(ZP1_VERSION "0.1.0")
endif()

project(ZP1 VERSION ${ZP1_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_PREFIX_PATH "/usr/local")
find_package(CycloneDDS-CXX REQUIRED)

add_subdirectory(logsystem)
add_subdirectory(idl)

option(ZP1_BUILD_GCONTROL "Build gcontrol process" ON)
if(ZP1_BUILD_GCONTROL)
  add_subdirectory(gcontrol)
endif()

option(ZP1_BUILD_HOST "Build host process" ON)
if(ZP1_BUILD_HOST)
  add_subdirectory(host)
endif()
