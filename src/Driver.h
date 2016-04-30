/*
    Copyright (C) 2016 Rafał Cieślak

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


#ifndef __DRIVER_HPP__
#define __DRIVER_HPP__

#include <algorithm>

#define BUFFER_SIZE 128

class AudioDriver{
public:
    // Adds a sample into internal buffer.
    virtual void AppendSample(double l, double r) = 0;
    // Sends samples stored in internal buffer for playback.
    virtual void CommitBuffer() = 0;
    // Blocks until all sent bytes have been played back, or until timeout (ms).
    // Shall return 1 if actually drained, or 0 in other case (e.g. timed out).
    virtual int Drain(int timeout) = 0;
    virtual ~AudioDriver(){}
};

class MidiDriver{
public:
    virtual int GetNotesOn(int channel) = 0;
    // Returns a pair: pitch, velocity
    virtual std::pair<int,int> GetLastNote(int channel) = 0;
    virtual ~MidiDriver(){}
};

struct AudioContext{
    AudioDriver* audio_driver;
    MidiDriver* midi_driver;
};

#endif // __DRIVER_HPP__
