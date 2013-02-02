/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#ifndef OUTLET_H
#define OUTLET_H

#include <cairomm/cairomm.h>

class Module;
class Wire;

class Outlet
{
    public:
        Outlet(Module* _parent, int _x, int _y);
        virtual ~Outlet();

        Module* parent;
        int x;
        int y;

        bool highlight;

        Wire* connection;
        void push_sample(double q);

        bool is_point_within(double x, double y);

        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
    private:
};

#endif // OUTLET_H
