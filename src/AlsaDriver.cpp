/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#include <iostream>
#include <gtkmm/main.h>
#include <alsa/asoundlib.h>
#include <cmath>
#include <cstring>
#include "AlsaDriver.h"
#include "Engine.h"

#define BUFFER_SIZE 128

extern bool quit_threads;

namespace AlsaDriver{

snd_pcm_t *pcm_handle;
snd_seq_t *seq_handle;

short sound_buffer[BUFFER_SIZE*2];

int buf_ptr = 0;

void open_seq(){
    if(snd_seq_open(&seq_handle,"default",SND_SEQ_OPEN_INPUT,0) < 0){
        std::cout << "Failed to open ALSA sequencer.\n";
        return;
    }
    snd_seq_set_client_name(seq_handle,"vmodsynth");
    snd_seq_create_simple_port(seq_handle, "vmodsynth", SND_SEQ_PORT_CAP_WRITE|
                                                        SND_SEQ_PORT_CAP_SUBS_WRITE, SND_SEQ_PORT_TYPE_SYNTHESIZER|
                                                                                     SND_SEQ_PORT_TYPE_SOFTWARE|
                                                                                     SND_SEQ_PORT_TYPE_APPLICATION|
                                                                                     SND_SEQ_PORT_TYPE_MIDI_GENERIC);

}

void open_pcm(char *pcm) {

    pcm_handle = NULL;

    if (snd_pcm_open(&pcm_handle,pcm,SND_PCM_STREAM_PLAYBACK,0) < 0){
        std::cout << "Failed to open PCM device '" << pcm << "'. Either the device is busy and you need to close other application using it, or try launching the application with different device as a command-line argument, e.g. 'vmodsynth hw:0,0'.\n";
        return;
    }
    snd_pcm_hw_params_t *hwp;
    snd_pcm_sw_params_t *swp;

    snd_pcm_hw_params_alloca(&hwp);
    snd_pcm_hw_params_any(pcm_handle, hwp);
    snd_pcm_hw_params_set_access(pcm_handle, hwp, SND_PCM_ACCESS_RW_INTERLEAVED); // interlaved channel samples layout in frames
    snd_pcm_hw_params_set_format(pcm_handle, hwp, SND_PCM_FORMAT_S16_LE); //16 bits per sample
    unsigned int rate = 44100;
    snd_pcm_hw_params_set_rate_near(pcm_handle, hwp, &rate, 0);
    snd_pcm_hw_params_set_channels(pcm_handle, hwp, 2);
    snd_pcm_hw_params_set_periods(pcm_handle, hwp, 1, 0);
    snd_pcm_hw_params_set_period_size(pcm_handle, hwp, BUFFER_SIZE, 0);
    snd_pcm_hw_params(pcm_handle, hwp);

    snd_pcm_sw_params_alloca(&swp);
    snd_pcm_sw_params_current(pcm_handle,swp);
    snd_pcm_sw_params_set_avail_min(pcm_handle,swp,BUFFER_SIZE);
    snd_pcm_sw_params(pcm_handle,swp);

}

const short short_max = ((int)2<<14) - (int)1;
const short short_min = ((int)2<<14) + (int)1;

void add_sample(double l, double r){
    if(buf_ptr >= 2*BUFFER_SIZE) return;
    //std::cout << "adding sample " << buf_ptr << ": " << q << " (to "<< sound_buffer[buf_ptr  ] << ")" << "\n";
    int s_l = (l)*(2<<14); // 2<<16 is the range of short. In fact, a 4 times smaller range is used here, so that it covers negative q value too (plus one bit is in fact lost because of the way ALSA uses the buffer)
    if (l >= 1.0) s_l = short_max;
    int s_r = (r)*(2<<14);
    if (r >= 1.0) s_r = short_max;

    /*s += sound_buffer[buf_ptr  ];
    if (s > short_max) s = short_max;
    if (s < short_min) s = short_min;
    sound_buffer[buf_ptr  ] = s;
    s += sound_buffer[buf_ptr+1];
    if (s > short_max) s = short_max;
    if (s < short_min) s = short_min;
    sound_buffer[buf_ptr+1] = s; */

    sound_buffer[buf_ptr   ] += s_l;
    sound_buffer[buf_ptr +1] += s_r;
    //std::cout << "now " << sound_buffer[buf_ptr  ] << "\n";
}

int last_note_pitch[17];
int last_note_velocity[17];
int notes_on[17];

void midi_input(){
    snd_seq_event_t* ev;
    do{
        snd_seq_event_input(seq_handle,&ev);
        int channel = ev->data.note.channel + 1;
        switch(ev->type){
            case SND_SEQ_EVENT_NOTEON:
                if(ev->data.note.velocity != 0){
                    last_note_pitch[0]    = ev->data.note.note;
                    last_note_velocity[0] = ev->data.note.velocity;
                    last_note_pitch[channel]    = ev->data.note.note;
                    last_note_velocity[channel] = ev->data.note.velocity;
                    notes_on[0]++;
                    notes_on[channel]++;
                }else{
                    notes_on[0]--;
                    notes_on[channel]--;
                    if(notes_on[0] == 0)
                        last_note_velocity[0] = 0;
                    if(notes_on[channel] == 0)
                        last_note_velocity[channel] = 0;
                }
                break;
            case SND_SEQ_EVENT_NOTEOFF:
                notes_on[0]--;
                notes_on[channel]--;
                if(notes_on[0] == 0)
                    last_note_velocity[0] = 0;
                if(notes_on[channel] == 0)
                    last_note_velocity[channel] = 0;
                break;
            case SND_SEQ_EVENT_CONTROLLER:
                for(auto &k : Engine::knobs){
                    if(k->controlled && (k->channel == 0 || k->channel == ev->data.control.channel + 1) && k->controller == ev->data.control.param){
                        k->set_value_from_controller(ev->data.control.value);
                    }
                }
                break;
        }
    }while (snd_seq_event_input_pending(seq_handle,0) > 0);
}

int get_last_note_pitch(int ch){
    return last_note_pitch[ch];
}

int get_last_note_velocity(int ch){
    return last_note_velocity[ch];
}

int get_notes_on(int ch){
    return last_note_velocity[ch];
}

int playback () {

    //This is the single DSP iteration.

    //Begin by freezing the GTK/GDK internal loops, so that they do not cause race conditions while calculations take place.
    gdk_threads_enter();


    memset(sound_buffer,0,BUFFER_SIZE*2*sizeof(short));

    //std::cout << "cleared.\n";

    //As the main step, do BUFFER_SIZE cycles of modules' DSP processing to produce this many samples in the output buffer.
    buf_ptr = 0;
    for(int x = 0; x < BUFFER_SIZE; x++){

        //Clear the buffer.

        Engine::do_dsp_cycle();
        buf_ptr += 2;
    }

    //Remove GTK/GDK threads lock.
    gdk_threads_leave();

    //Finally, pass the contents of the soundbuffer to ALSA.
    return snd_pcm_writei(pcm_handle, sound_buffer, BUFFER_SIZE);
}


void thread_main(char* device){

    open_pcm(device);
    open_seq();
    if(!pcm_handle || !seq_handle) return;

    int nfds, seq_nfds, l1;
    struct pollfd *pfds;

    nfds = snd_pcm_poll_descriptors_count(pcm_handle);
    seq_nfds = snd_seq_poll_descriptors_count(seq_handle,POLLIN);
    pfds = (struct pollfd *)alloca(sizeof(struct pollfd) * (nfds + seq_nfds));
    snd_seq_poll_descriptors(seq_handle,pfds,seq_nfds,POLLIN);
    snd_pcm_poll_descriptors (pcm_handle, pfds + seq_nfds, nfds);

    while(!quit_threads){
        //poll for incomming pcm interrupts
        if (poll (pfds, seq_nfds + nfds, 1000) > 0) {
            //for each event
            for (l1 = 0; l1 < seq_nfds; l1++) {
                if (pfds[l1].revents > 0)
                    midi_input();
            }
            for (l1 = seq_nfds; l1 < seq_nfds + nfds; l1++){
                if (pfds[l1].revents > 0) {
                    //call playback callback
                    int r;
                    if ((r = playback()) < BUFFER_SIZE) {
                            snd_pcm_prepare(pcm_handle);
                            std::cout << "a XRUN occured! " << r << std::endl;
                    }
                }
            }
        }
    }

    snd_pcm_close(pcm_handle);
}

} //namespace AlsaDriver
