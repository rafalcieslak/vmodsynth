/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#ifndef V702_H
#define V702_H

#include "../Module.h"

/*
 * Overdrive module
 */

class v702 : public Module
{
    public:
        v702();
        virtual ~v702();
        void dsp();
        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
    private:
};

#endif // V702_H
