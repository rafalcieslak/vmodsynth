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


#include "Selector.h"
#include "Module.h"

Selector::Selector(Module *parent, int x, int y, int _vals, int def, bool _hide_val) : Knob(parent, x, y, 0.0, def, (double)_vals-0.99)
{
    vals = _vals;
    selector_value = (int)value;
    hide_val = _hide_val;
}

Selector::~Selector()
{
    //dtor
}
void Selector::movement_position(int x, int y){
    Knob::movement_position(x,y);
    selector_value = (int)value;
}

void Selector::set_value_from_controller(int v){
    Knob::set_value_from_controller(v);
    selector_value = (int)value;
}

double Selector::get_value(){
    return (double)selector_value;
}

void Selector::draw(const Cairo::RefPtr<Cairo::Context>& cr){


    cr->save();
    cr->translate(parent->xoffset + x, y);

    //silhouette
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->arc(0,0, size, 0.0, 2*M_PI);
    cr->fill();

    //marker
    double angle = 2*M_PI*((double)selector_value - min_val)/range;
    angle *= 0.5;
    angle += 0.75*2*M_PI;
    cr->save();
    cr->rotate(angle);

    cr->set_source_rgb(1.0, 1.0, 1.0);
    cr->set_line_width(3.0);
    cr->move_to(0,-size);
    cr->line_to(0,-size*0.3);
    cr->stroke();
    cr->restore();

    //inner circle
    if(controlled)
        cr->set_source_rgb(0.7, 0.3, 0.3);
    else
        cr->set_source_rgb(1.0, 1.0, 1.0);
    cr->arc(0,0, size*0.35, 0.0, 2*M_PI);
    cr->fill();

    char val[10];
    //value
    if(!hide_val){
        sprintf(val,"%d",selector_value);
        cr->set_font_size(10.0);
        //cr->select_font_face("Verdana",Cairo::FONT_SLANT_NORMAL,Cairo::FONT_WEIGHT_BOLD);
        Cairo::TextExtents tx;
        cr->get_text_extents(val,tx);
        cr->set_source_rgb(0.0,0.0,0.0);
        cr->move_to(- tx.width/2,tx.height/2);
        cr->show_text(val);
    }

    if(controlled){
        //controller data
        sprintf(val,"(%d: %d)",channel, controller);
        cr->set_font_size(10.0);
        Cairo::TextExtents tx;
        cr->get_text_extents(val,tx);
        cr->set_source_rgb(1.0,1.0,1.0);
        cr->move_to(- tx.width/2,((!hide_val)?10.0:0.0) + tx.height/2);
        cr->show_text(val);
    }

    cr->restore();
}
