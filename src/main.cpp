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
#include <atomic>
#include <unistd.h>
#include <iostream>
#include <getopt.h>

#include "MainWindow.h"
#include "AlsaDriver.h"
#include "Engine.h"

MainWindow *mainwindow;

std::atomic<bool> quit_threads(false);

void audio_main(AudioContext ctx){
    while(!quit_threads){
        ctx.audio_driver->CommitBuffer();

        for(int i = 0; i < BUFFER_SIZE; i++){
            // These may call audio_driver's AddSample.
            Engine::do_dsp_cycle(ctx);
        }

        while(!quit_threads && ! ctx.audio_driver->Drain(1000));
    }
}

void midi_main(AudioContext ctx){
    while(!quit_threads){
        ctx.midi_driver->ProcessInput();

        while(!quit_threads && ! ctx.midi_driver->WaitForInput(1000));
    }
}

void usage(const char* progname){
    std::cout <<
        "Usage: " << progname << " [OPTIONS] [FILENAME]\n"
        "\n"
        "Software modular synthesizer that aims to resemble look,\n"
        "feel and the workflow typical to analog modular synthesizers.\n"
        " --alsa        Enables ALSA audio driver (this is default).\n"
        " --pa, --pulseaudio\n"
        "               Enables PulseAudio audio driver (not yet\n"
        "               implemented).\n"
        " -d DEV, --device DEV\n"
        "               For ALSA driver, sets the output device name.\n"
        " -h, --help    Displays this text.\n";
}

int main(int argc, char *argv[]){

    int config_alsa = 0;
    int config_pa = 0;
    static struct option long_opts[] =
        {
            {"alsa", no_argument, &config_alsa, 1},
            {"pulseaudio", no_argument, &config_pa, 1},
            {"pa", no_argument, &config_pa, 1},
            {"device", no_argument, 0, 'd'},
            {"help", no_argument, 0, 'h'},
            {0,0,0,0}
        };

    std::string config_device = "default";

    int c, opt_index = 0;
    while((c = getopt_long(argc,argv,"hd:",long_opts,&opt_index)) != -1){
        switch (c){
        case 'h':
            usage(argv[0]);
            return 0;
            break;
        case 'd':
            config_device = optarg;
            break;
        default:
            std::cout << "Unrecognized option " << (char)c << std::endl;
            usage(argv[0]);
            return 1;
        }
    }

    std::vector<std::string> config_infiles;
    while(optind < argc){
        config_infiles.push_back(argv[optind]);
        optind++;
    }
    if(config_infiles.size() > 1){
        std::cout << "Unable to open multiple files at once." << std::endl;
        return 1;
    }

    Gtk::Main kit(argc,argv);

    AudioDriver* audio_driver = nullptr;
    MidiDriver*   midi_driver = nullptr;

    // Select audio driver to use
    if(!config_pa && !config_alsa){
        config_alsa = 1;
    }
    if(config_pa && config_alsa){
        std::cout << "Cannot enable multiple audio drivers at once." << std::endl;
        return 1;
    }
    if(config_pa){
        std::cout << "ERROR: Pulseaudio driver is not yet implemented." << std::endl;
        // audio_driver = new PulseaudioAlsaDriver();
        return 1;
    }else if(config_alsa){
        audio_driver = new AlsaAudioDriver(config_device);
    }

    // Select midi driver to use
    midi_driver = new AlsaSeqDriver();

    // Create audio context
    AudioContext ctx = {audio_driver, midi_driver};

    // TODO: Create mainwindow on stack here and pass a reference to Engine.
    mainwindow = new MainWindow();

    std::thread audio_thread(audio_main, ctx);
    std::thread  midi_thread( midi_main, ctx);

    if(config_infiles.size() > 0){
        // TODO: Open file.
    }

    Gtk::Main::run();

    // ======================

    quit_threads = true;
    audio_thread.join();
     midi_thread.join();

    delete mainwindow;
    delete midi_driver;
    delete audio_driver;

    return 0;
}
