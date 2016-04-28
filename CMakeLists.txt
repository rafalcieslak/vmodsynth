cmake_minimum_required(VERSION 2.8)
project (vmodsynth C CXX) # The sources use C++ exclusivelly, but CMake needs to
# have C enabled too, in order to correctly detect pthreds library.

if(DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: Debug Release None(CMAKE_CXX_FLAGS and CMAKE_C_FLAGS used) .")
else()
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: Debug Release or None(CMAKE_CXX_FLAGS and CMAKE_C_FLAGS used) .")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "Current build mode: ${CMAKE_BUILD_TYPE}")



add_subdirectory (src)
