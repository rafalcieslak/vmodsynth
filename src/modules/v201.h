/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#ifndef V201_H
#define V201_H

#include "../Module.h"

/*
 * Panorama (splitter) module
 */
class v201 : public Module
{
    public:
        v201();
        virtual ~v201();
        void dsp();
        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
    private:
};

#endif // V201_H
