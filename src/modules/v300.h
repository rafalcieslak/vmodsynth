/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#ifndef V300_H
#define V300_H

#include <complex>
#include "../Module.h"

/*
 *  Variable State Filter module
 */
class v300 : public Module
{
    public:
        v300();
        virtual ~v300();
        void dsp();
        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
    private:
        double freq_last;

        double hip_last;
        double lop_last;

        double bp_last;
        double bp_prev;

        double biq[10][3];
};

#endif // V300_H
