/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#ifndef V101_H
#define V101_H

#include "../Module.h"


class v101 : public Module
{
    public:
        v101();
        virtual ~v101();
        void dsp();
        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
    private:
        double b0,b1,b2,b3,b4,b5,b6;
};

#endif // V101_H
