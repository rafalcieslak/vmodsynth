/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#ifndef V203_H
#define V203_H

#include "../Module.h"

/*
 * Sample and hold module
 */
class v203 : public Module
{
    public:
        v203();
        virtual ~v203();
        void dsp();
        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
    private:
        double last_input;
        double held_sample;
        double c;
};

#endif // V203_H
