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


#ifndef SELECTOR_H
#define SELECTOR_H

#include "Knob.h"

class Module;

class Selector : public Knob
{
    public:
        Selector(Module *parent, int x, int y, int _vals, int def, bool _hide_val = false);
        virtual ~Selector();

        int vals;
        int selector_value;

        void movement_position(int x, int y);

        double get_value();

        void set_value(double val);

        void set_value_from_controller(int v);

        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
    private:
        bool hide_val;
};

#endif // SELECTOR_H
