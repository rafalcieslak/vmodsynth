/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
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

        void set_value_from_controller(int v);

        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
    private:
        bool hide_val;
};

#endif // SELECTOR_H
