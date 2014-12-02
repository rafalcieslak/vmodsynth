bin_PROGRAMS = vmodsynth
vmodsynth_SOURCES = AlsaDriver.cpp \
                    Cabinet.cpp \
                    ControllerWindow.cpp \
                    Engine.cpp \
                    Inlet.cpp \
                    Knob.cpp \
                    main.cpp \
                    MainWindow.cpp \
                    Module.cpp \
                    ModuleList.cpp \
                    Outlet.cpp \
                    Selector.cpp \
                    Switch.cpp \
                    Wire.cpp \
                    modules/algorithms.cpp  modules/v1001.cpp  modules/v1005.cpp  modules/v100.cpp  modules/v101.cpp  modules/v200.cpp  modules/v201.cpp  modules/v203.cpp  modules/v210.cpp  modules/v230.cpp  modules/v300.cpp  modules/v400.cpp  modules/v701.cpp  modules/v702.cpp  modules/v703.cpp \
                    AlsaDriver.h Cabinet.h ControllerWindow.h Engine.h Inlet.h Knob.h  MainWindow.h Module.h ModuleList.h Outlet.h Selector.h Switch.h Wire.h modules/algorithms.h  modules/v1001.h  modules/v1005.h  modules/v100.h  modules/v101.h  modules/v200.h  modules/v201.h  modules/v203.h  modules/v210.h  modules/v230.h  modules/v300.h  modules/v400.h  modules/v701.h  modules/v702.h  modules/v703.h

vmodsynth_CPPFLAGS = $(GTKMM_CFLAGS) -O3
vmodsynth_LDADD = $(GTKMM_LIBS) $(CAIROMM_LIBS) -lasound
vmodsynth_LDFLAGS = -pthread
