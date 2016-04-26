/*
    Copyright (C) 2012, 2013 Rafał Cieślak

    This file is part of vModSynth.

    vModSynth is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    vModSynth is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with vModSynth.  If not, see <http://www.gnu.org/licenses/>.
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
        void set_value(double val); //RG 2016

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
        int subtype; //added RG2016 for reloading of settings from XML-file
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
