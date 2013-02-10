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


#include "v701.h"

v701::v701()
{
    name = "Echo effect";
    caption = "ECHO";
    type_id = 701;
    panel_width = 136;
    add_inlet(33 ,588);
    add_outlet(103,588);
    add_knob(68,107,0.0,5.0,10.0);
    add_knob(68,230,0.0,55.0,100.0,10);
}

v701::~v701()
{
    //dtor
}

void v701::dsp(){
    double in = inlets[0]->pull_sample();
    double delay = pow(6.32455532,knobs[0]->get_value()/5.0) * 0.05;
    delay = delay*44100.0;
    int time = delay;
    double add = 0.0;
    while(del.size() > time) del.pop();
    if(del.size() == time){
        add = del.front();
        del.pop();
    }
    double feedback = knobs[1]->get_value()/100.0;
    del.push( (in + add) * feedback);
    outlets[0]->push_sample(in+add);
}

void v701::draw(const Cairo::RefPtr<Cairo::Context>& cr){
    draw_background(cr,0.5);
    draw_caption_and_id(cr);

    cr->save();
    cr->translate(xoffset,0);

    cr->set_source_rgb(1.0,1.0,1.0);
    cr->set_line_width(2.5);
    cr->set_line_cap(Cairo::LINE_CAP_ROUND);
    cr->move_to(12,532);
    cr->line_to(panel_width - 12, 532);
    cr->stroke();

    draw_text(cr,"TIME",10.0, 68,  45);
    draw_text(cr,"FEEDBACK (%)",10.0, 68, 168);
    draw_text(cr,"INPUT"  ,10.0, 33,  554);
    draw_text(cr,"OUTPUT" ,10.0, 103, 554);

    cr->restore();
}
