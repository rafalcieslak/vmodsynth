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


#include "v203.h"

v203::v203()
{
    name = "Sample and hold";
    caption = "SAMPLE & HOLD";
    type_id = 203;
    panel_width = 136;
    add_inlet(33 ,513); //gate
    add_inlet(103,513); //input
    //add_outlet(33 ,588);
    add_outlet(103,588); //output
    add_knob(68,107,0.0,10.0,10.0); // input level
    add_knob(68,230,0.0,0.0,10.0); // sample rate
    add_switch(33,410,"INTERNAL","EXTERNAL",true);
    last_input = 0.0;
    held_sample = 0.0;
    c = 0.0;
}

v203::~v203()
{
    //dtor
}

void v203::dsp(){
    bool external = switches[0]->get_value();
    double multiplier = knobs[0]->get_value();
    double new_input = inlets[0]->pull_sample();
    if(external){
        if(new_input > last_input) held_sample = inlets[1]->pull_sample() * multiplier;
    }else{ //internal
        double freq = 1.118033989 * pow(22.360679775,(knobs[1]->get_value()/5.0)-1.0);
        double d = freq/44100.0;
        c += d;
        if(c >= 1.0){
            c -= 1.0;
            held_sample = inlets[1]->pull_sample() * multiplier;
        }
    }
    last_input = new_input;
    outlets[0]->push_sample(held_sample);
}

void v203::draw(const Cairo::RefPtr<Cairo::Context>& cr){
    draw_background(cr,0.3);
    draw_caption_and_id(cr);
    cr->save();
    cr->translate(xoffset,0);
    draw_text(cr,"OUTPUT" ,10.0, 103, 554);
    draw_text(cr,"GATE" ,10.0, 33, 479);
    draw_text(cr,"INPUT" ,10.0, 103, 479);
    draw_text(cr,"INPUT LEVEL",10.0, 68,  45);
    draw_text(cr,"SAMPLE RATE",10.0, 68, 168);

    cr->set_line_width(2.0);
    cr->set_line_cap(Cairo::LINE_CAP_ROUND);
    cr->set_source_rgb(1.0,1.0,1.0);

    cr->move_to(33,445);
    cr->line_to(33,469);
    cr->stroke();

    cr->move_to(33,372);
    cr->line_to(33,300);
    cr->line_to(68,300);
    cr->line_to(68,230);
    cr->stroke();

    cr->restore();
}
