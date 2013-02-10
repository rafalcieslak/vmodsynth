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
