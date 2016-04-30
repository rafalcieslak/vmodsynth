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


#include <iostream>
#include <cstring>
#include "AlsaDriver.h"
#include "Engine.h"

AlsaAudioDriver::AlsaAudioDriver(std::string device){

    pcm_handle = nullptr;

    if (snd_pcm_open(&pcm_handle, device.c_str(), SND_PCM_STREAM_PLAYBACK, 0) < 0){
        std::cout << "Failed to open PCM device '" << device << "'. Either the device is busy and you need to close other application using it, or try launching the application with different device as a command-line argument, e.g. 'vmodsynth hw:0,0'.\n";
        // TODO: Throw an exception.
        return;
    }
    snd_pcm_hw_params_t *hwp;
    snd_pcm_sw_params_t *swp;

    snd_pcm_hw_params_alloca(&hwp);
    snd_pcm_hw_params_any(pcm_handle, hwp);
    // interlaved channel samples layout in frames
    snd_pcm_hw_params_set_access(pcm_handle, hwp, SND_PCM_ACCESS_RW_INTERLEAVED);
    // 16 bits per sample
    snd_pcm_hw_params_set_format(pcm_handle, hwp, SND_PCM_FORMAT_S16_LE);
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

    bzero(buffer,BUFFER_SIZE*2*sizeof(short));
}

AlsaAudioDriver::~AlsaAudioDriver(){
    if(pcm_handle)
        snd_pcm_close(pcm_handle);
}

void AlsaAudioDriver::CommitBuffer(){
    if(!pcm_handle) return;

    int r = snd_pcm_writei(pcm_handle, buffer, BUFFER_SIZE);
    if(r < BUFFER_SIZE){
        snd_pcm_prepare(pcm_handle);
        std::cout << "a XRUN occured! " << r << std::endl;
    }

    buffer_pos = 0;
    bzero(buffer,BUFFER_SIZE*2*sizeof(short));
}

int AlsaAudioDriver::Drain(int timeout){
    if(!pcm_handle) return 0;

    int nfds;
    struct pollfd *pfds;

    nfds = snd_pcm_poll_descriptors_count(pcm_handle);
    if(nfds != 1){
        std::cout << "WARNING: nfds != 1." << std::endl;
    }
    pfds = (struct pollfd *)alloca(sizeof(struct pollfd) * (nfds));
    snd_pcm_poll_descriptors(pcm_handle, pfds, nfds);

    int n = poll (pfds, nfds, timeout);
    if(n <= 0) return 0;

    return 1;
}

void AlsaAudioDriver::AppendSample(double l, double r){
    if(buffer_pos >= 2*BUFFER_SIZE) return;

    // 2<<16 is the range of short. In fact, a 4 times smaller range
    // is used here, so that it covers negative q value too (plus one
    // bit is in fact lost because of the way ALSA uses the buffer)
    int s_l = (l)*(2<<14);
    if (l >= 1.0) s_l = std::numeric_limits<short>::max();
    int s_r = (r)*(2<<14);
    if (r >= 1.0) s_r = std::numeric_limits<short>::max();

    // TODO: Proper clipping?

    buffer[buffer_pos    ] += s_l;
    buffer[buffer_pos +1 ] += s_r;

    buffer_pos += 2;
}


AlsaSeqDriver::AlsaSeqDriver(){
    if(snd_seq_open(&seq_handle,"default",SND_SEQ_OPEN_INPUT,0) < 0){
        std::cout << "Failed to open ALSA sequencer.\n";
        // TODO: Throw an exception?
        return;
    }
    snd_seq_set_client_name(seq_handle,"vmodsynth");
    snd_seq_create_simple_port(seq_handle,
                               "vmodsynth",
                               SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
                               SND_SEQ_PORT_TYPE_SYNTHESIZER|
                               SND_SEQ_PORT_TYPE_SOFTWARE|
                               SND_SEQ_PORT_TYPE_APPLICATION|
                               SND_SEQ_PORT_TYPE_MIDI_GENERIC);
    bzero(last_note_pitch,17*sizeof(int));
    bzero(last_note_velocity,17*sizeof(int));
    bzero(notes_on,17*sizeof(int));
}
AlsaSeqDriver::~AlsaSeqDriver(){
    if(seq_handle)
        snd_seq_close(seq_handle);
}

int AlsaSeqDriver::GetNotesOn(int channel){
    return notes_on[channel] != 0;
}

std::pair<int,int> AlsaSeqDriver::GetLastNote(int channel){
    return {last_note_pitch[channel], last_note_velocity[channel]};
}

void AlsaSeqDriver::ProcessInput(){
    if(!seq_handle) return;
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

int AlsaSeqDriver::WaitForInput(int timeout){
    if(!seq_handle) return 0;

    int nfds;
    struct pollfd *pfds;

    nfds = snd_seq_poll_descriptors_count(seq_handle,POLLIN);
    pfds = (struct pollfd *)alloca(sizeof(struct pollfd) * (nfds));
    snd_seq_poll_descriptors(seq_handle,pfds,nfds,POLLIN);

    int n = poll (pfds, nfds, timeout);

    if(n <= 0) return 0;

    return 1;
}
