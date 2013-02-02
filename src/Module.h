/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
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
    protected:
        int add_inlet(int x, int y);
        int add_outlet(int x, int y);
        int add_knob(int x, int y, double _min, double _value, double _max, int steps = 0);
        int add_selector(int x, int y, int vals, int def, bool hide = true);
        int add_switch(int x, int y, std::string text1 = "OFF", std::string text2 = "ON", bool vertival = false, bool val = false);

        //Drawing helper functions
        void draw_background(const Cairo::RefPtr<Cairo::Context>& cr, double grayness);
        void draw_caption_and_id(const Cairo::RefPtr<Cairo::Context>& cr);
        void draw_text(const Cairo::RefPtr<Cairo::Context>& cr, const char *text, double size, double x, double y, double xscale = 1.0, double r = 1.0, double g = 1.0, double b = 1.0);
    private:
};

#endif // MODULE_H
