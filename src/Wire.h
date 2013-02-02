/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#ifndef WIRE_H
#define WIRE_H

#include <cairomm/cairomm.h>

class Inlet;
class Outlet;

class Wire
{
    public:
        Wire(Outlet* _from, Inlet* _to);
        virtual ~Wire();

        Outlet* from;
        Inlet* to;

        double in_sample;
        double out_sample;
        void pass_sample();

        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
    private:
        double g_delta;
        double b_delta;
};

#endif // WIRE_H
