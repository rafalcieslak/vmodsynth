/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#ifndef V1005_H
#define V1005_H

#include "../Module.h"

/*
 * Pseudo-module that reacts on ALSA MIDI input
 */
class v1005 : public Module
{
    public:
        v1005();
        virtual ~v1005();
        void dsp();
        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
    private:
        int last_gate_pitch;
};

#endif // V1005_H
