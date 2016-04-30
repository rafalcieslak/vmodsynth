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

namespace AlsaDriver{

int get_last_note_pitch(int ch);
int get_last_note_velocity(int ch);
int get_notes_on(int ch);

}

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

#endif // ALSA_DRIVER_H
