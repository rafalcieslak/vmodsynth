/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#ifndef SWITCH_H
#define SWITCH_H

#include <cairomm/cairomm.h>
#include <glibmm.h>

class Module;

class Switch
{
    public:
        Switch(Module* _parent, int _x, int _y, std::string _text1 = "OFF", std::string _text2 = "ON", bool vertical = false, bool val = false);
        virtual ~Switch();

        bool get_value();
        void set_value(bool v);

        Module* parent;
        int x;
        int y;

        std::string text1;
        std::string text2;

        bool is_point_within(double x, double y);

        Glib::Dispatcher redraw_me; //for thread-safe refreshing
        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
        bool value;
        bool vertical;

        void redraw_me_real();
    private:
};

#endif // SWITCH_H
