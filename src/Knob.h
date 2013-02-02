/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#ifndef KNOB_H
#define KNOB_H

#include <cairomm/cairomm.h>
#include <glibmm.h>

class Module;

class Knob
{
    public:
        Knob(Module* _parent, int _x, int _y, double _min, double _value, double _max, int _steps = 0);
        virtual ~Knob();

        Module *parent;

        virtual double get_value();

        bool is_point_within(double x, double y);

        void movement_start();
        virtual void movement_position(int x, int y);
        void movement_end();
        double movement_value;

        Glib::Dispatcher redraw_me; //for thread-safe refreshing
        virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr);

        double size;

        bool controlled;
        int channel;
        int controller;
        virtual void set_value_from_controller(int v);

    protected:
        int x;
        int y;

        double min_val;
        double value;
        double max_val;
        int steps;
        double range;

        void redraw_me_real();

    private:

};

#endif // KNOB_H
