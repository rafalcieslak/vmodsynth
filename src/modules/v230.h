/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#ifndef V230_H
#define V230_H

#include "../Module.h"

/*
 * ADSR envelope generator module
 */
class v230 : public Module
{
    public:
        v230();
        virtual ~v230();
        void dsp();
        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
    private:
        double current_val;
        double last_input;
        int phase;

        double d;
};

#endif // V230_H
