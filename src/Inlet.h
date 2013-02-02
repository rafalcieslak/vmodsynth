/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#ifndef INLET_H
#define INLET_H

#include <cairomm/cairomm.h>

class Module; //Avoiding recursive including
class Wire;

class Inlet
{
    public:
        Inlet(Module* _parent, int _x, int _y);
        virtual ~Inlet();

        Module* parent;
        int x;
        int y;

        bool highlight;

        Wire* connection;
        double pull_sample();

        bool is_point_within(double x, double y);

        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
    private:
};

#endif // INLET_H
