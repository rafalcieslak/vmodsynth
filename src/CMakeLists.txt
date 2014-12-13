
find_package(PkgConfig REQUIRED)
find_package(ALSA REQUIRED)
find_package(Threads REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0 REQUIRED)
pkg_check_modules(CAIROMM cairomm-1.0 REQUIRED)

add_definitions(
	--std=c++11
	-Wall
	-Wextra
#	-Werror
	-g
)

link_directories(
    ${ALSA_LIBRARY_DIRS}
    ${GTKMM_LIBRARY_DIRS}
    ${CAIROMM_LIBRARY_DIRS}
)

include_directories(
    ${ALSA_INCLUDE_DIRS}
    ${GTKMM_INCLUDE_DIRS}
    ${CAIROMM_INCLUDE_DIRS}
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
	${CMAKE_THREAD_LIBS_INIT}
)

# When installing, move vmodsynth binary to the bin directory
install(TARGETS vmodsynth
        RUNTIME DESTINATION bin
)
