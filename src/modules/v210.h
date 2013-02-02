/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#ifndef V210_H
#define V210_H

#include "../Module.h"

/*
 * Amplifier (Voltage Controlled Attenuator) module
 */
class v210 : public Module
{
    public:
        v210();
        virtual ~v210();
        void dsp();
        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
    private:
};

#endif // V210_H
