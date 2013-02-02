/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#ifndef V703_H
#define V703_H

#include "../Module.h"
#include <queue>
/*
 * Artificial reverbator module
 */

class v703 : public Module
{
    public:
        v703();
        virtual ~v703();
        void dsp();
        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
    private:
        std::queue<double> e1, e2, e3;
        std::queue<double> m1, m2, m3, m4;
};

#endif // V703_H
