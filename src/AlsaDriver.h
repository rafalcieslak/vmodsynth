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

#ifndef ALSA_DRIVER_H
#define ALSA_DRIVER_H

#include <string>
#include <alsa/asoundlib.h>

#include "Driver.h"

class AlsaAudioDriver : public AudioDriver{
public:
    AlsaAudioDriver(std::string device);
    virtual void AppendSample(double l, double r);
    virtual void CommitBuffer();
    virtual int Drain(int timeout);
    virtual ~AlsaAudioDriver();
private:
    snd_pcm_t *pcm_handle;
    short buffer[2*BUFFER_SIZE];
    unsigned int buffer_pos = 0;
};

class AlsaSeqDriver : public MidiDriver{
public:
    AlsaSeqDriver();
    virtual int WaitForInput(int timeout);
    virtual void ProcessInput();

    virtual int GetNotesOn(int channel);
    // Returns a pair: pitch, velocity
    virtual std::pair<int,int> GetLastNote(int channel);
    virtual ~AlsaSeqDriver();
private:
    snd_seq_t *seq_handle;
    int last_note_pitch[17];
    int last_note_velocity[17];
    int notes_on[17];
};

#endif // ALSA_DRIVER_H
