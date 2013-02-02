/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#ifndef V100_H
#define V100_H

#include "../Module.h"

/*
 * Oscillator (+VCO+LFO) module
 */
class v100 : public Module
{
    public:
        v100();
        virtual ~v100();
        void dsp();
        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
    private:
        double c;
        double freq;
        double last_hs_in;
};

#endif // V100_H
