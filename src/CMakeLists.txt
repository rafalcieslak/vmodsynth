find_package(PkgConfig REQUIRED)
find_package(ALSA REQUIRED)
find_package(Threads REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0 REQUIRED)
pkg_check_modules(CAIROMM cairomm-1.0 REQUIRED)

#Use the local script (in src/) to find libXml++
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Xml++ REQUIRED)

add_definitions(
  --std=c++0x #Wass c++11, Works on Ubuntu 14, but did not run on Ubuntu 13
  )

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  add_definitions(-g -Og -Wall -Wextra -Werror)
endif()

if(${CMAKE_BUILD_TYPE} MATCHES "Release")
  add_definitions(-O3 -flto)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -flto")
endif()


link_directories(
    ${ALSA_LIBRARY_DIRS}
    ${GTKMM_LIBRARY_DIRS}
    ${CAIROMM_LIBRARY_DIRS}
)

include_directories(
    ${ALSA_INCLUDE_DIRS}
    ${GTKMM_INCLUDE_DIRS}
    ${CAIROMM_INCLUDE_DIRS}
    /usr/include/libxml++-2.6/libxml++/ #The automagic does not get this right
    /usr/lib/libxml++-2.6/include       #dito
)

# Get the list of all module files
file(GLOB MODULES modules/*.cpp)
# Get the list of all core sources
file(GLOB SOURCES ./*.cpp)

add_executable(vmodsynth
	${SOURCES}
	${MODULES}
)

target_link_libraries( vmodsynth
    ${ALSA_LIBRARIES}
    ${GTKMM_LIBRARIES}
    ${CAIROMM_LIBRARIES}
    /usr/lib/libxml++-2.6.so #Here we go again...
    ${CMAKE_THREAD_LIBS_INIT}
)

add_custom_command(
	TARGET vmodsynth POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/vmodsynth${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_BINARY_DIR}/"
    )

# When installing, move vmodsynth binary to the bin directory
install(TARGETS vmodsynth
        RUNTIME DESTINATION bin
)
