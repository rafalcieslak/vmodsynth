AC_INIT([vmodsynth],[1.0])
AM_INIT_AUTOMAKE([foreign subdir-objects -Wall])

AC_LANG([C++])
CXXFLAGS="$CXXFLAGS -std=c++0x"

AC_CHECK_LIB([asound], [snd_seq_open], [] , [
					echo "Alsa library missing - install alsa developement headers! [libasound2-dev on Ubuntu]"
					exit -1 ])
PKG_CHECK_MODULES([GTKMM], [gtkmm-3.0], [] , [
					echo "Gtkmm 3.0 library missing - install libgtkmm-3.0 developement headers! [libgtkmm-3.0-dev on Ubuntu]"
					exit -1 ])
PKG_CHECK_MODULES([CAIROMM], [cairomm-1.0], [] , [
					echo "Cairomm library missing - install libcairomm-1.0 developement headers! [libcairomm-1.0-dev on Ubuntu]"
					exit -1 ])
					
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
