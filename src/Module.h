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


#ifndef MODULE_H
#define MODULE_H
#include <string>
#include <vector>
#include <cairomm/cairomm.h>
#include "Inlet.h"
#include "Outlet.h"
#include "Knob.h"
#include "Switch.h"

#define MODULE_PANEL_HEIGHT 630

class Module
{
    public:
        Module();
        virtual ~Module();
        Module(const Module& other);

        std::string name;
        std::string caption;
        int type_id; //ModuleList.h

        virtual void dsp()=0;

        std::vector<Inlet *> inlets;
        std::vector<Outlet *> outlets;
        std::vector<Knob *> knobs;
        std::vector<Switch *> switches;

        int xoffset;

        bool is_point_within(double x, double y);

        int panel_width;
        virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr)=0; //pure virtual

        bool highlight;

        Inlet* get_inlet_at(int x, int y);  
        Outlet* get_outlet_at(int x, int y);
        int get_inlet_n_at(int x, int y);  
        int get_outlet_n_at(int x, int y);

    protected:
        void add_inlet(int x, int y);
        void add_outlet(int x, int y);
        void add_knob(int x, int y, double _min, double _value, double _max, int steps = 0);
        void add_selector(int x, int y, int vals, int def, bool hide = true);
        void add_switch(int x, int y, std::string text1 = "OFF", std::string text2 = "ON", bool vertival = false, bool val = false);

        //Drawing helper functions
        void draw_background(const Cairo::RefPtr<Cairo::Context>& cr, double grayness);
        void draw_caption_and_id(const Cairo::RefPtr<Cairo::Context>& cr);
        void draw_text(const Cairo::RefPtr<Cairo::Context>& cr, const char *text, double size, double x, double y, double xscale = 1.0, double r = 1.0, double g = 1.0, double b = 1.0);
    private:
};

#endif // MODULE_H
