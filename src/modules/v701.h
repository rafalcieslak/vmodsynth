/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#ifndef V701_H
#define V701_H

#include "../Module.h"
#include <queue>

/*
 * Echo effect module
 */

class v701 : public Module
{
    public:
        v701();
        virtual ~v701();
        void dsp();
        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
    private:
        std::queue<double> del;

};

#endif // V701_H
