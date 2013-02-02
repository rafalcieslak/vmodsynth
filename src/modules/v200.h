/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#ifndef V200_H
#define V200_H

#include "../Module.h"

/*
 * Multiplier module (directs signal to multiple intlets)
 */
class v200 : public Module
{
    public:
        v200();
        virtual ~v200();
        void dsp();
        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
    private:
};

#endif // V200_H
