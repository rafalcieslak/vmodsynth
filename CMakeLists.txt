cmake_minimum_required(VERSION 2.8)
project (vmodsynth C CXX) # The sources use C++ exclusivelly, but CMake needs to
# have C enabled too, in order to correctly detect pthreds library.

add_subdirectory (src)
