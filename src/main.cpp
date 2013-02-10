/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#include <gtkmm/main.h>
#include <thread>
#include <unistd.h>
#include "MainWindow.h"
#include "AlsaDriver.h"

MainWindow *mainwindow;

bool quit_threads = false;

char device[100] = "default";

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
