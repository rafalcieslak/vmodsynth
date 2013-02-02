/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#ifndef V1001_H
#define V1001_H

#include "../Module.h"

/*
 * Pseudo-module that passes output to ALSA
 */
class v1001 : public Module
{
    public:
        v1001();
        virtual ~v1001();
        void dsp();
        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
    private:
};

#endif // V1001_H
