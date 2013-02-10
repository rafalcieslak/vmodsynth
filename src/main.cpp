/*
    Copyright (C) 2012, 2013 Rafał Cieślak

    This file is part of vModSynth.

    vModSynth is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    vModSynth is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with vModSynth.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <gtkmm/main.h>
#include <thread>
#include <unistd.h>
#include "MainWindow.h"
#include "AlsaDriver.h"

MainWindow *mainwindow;

bool quit_threads = false;

char device[100] = "plughw:0,0";

void audio_thread_func(){
    AlsaDriver::thread_main(device);
}

int main(int argc, char *argv[]){

    if(argc > 1){
        strcpy(device,argv[1]);
    }

    g_type_init();
    gdk_threads_init();

    Gtk::Main kit(argc,argv);

    std::thread audio_thread(audio_thread_func);

    mainwindow = new MainWindow;

    gdk_threads_enter();
    Gtk::Main::run();
    gdk_threads_leave();

    quit_threads = true;
    audio_thread.join();

    delete mainwindow;
    return 0;
}
