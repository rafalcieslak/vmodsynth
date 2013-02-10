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


#include "v201.h"

v201::v201()
{
    name = "Panorama & crossafade";
    caption = "PAN/FADE";
    type_id = 201;
    panel_width = 136;

    //crossfade section
    add_inlet(33,85);     // 0
    add_inlet(103,85);    // 1
    add_outlet(68 ,149);  //    0
    add_knob(68,236,-5.0,0.0,5.0);


    //fade section
    add_inlet(68 ,524);   // 2
    add_outlet(33 ,588);  //    1
    add_outlet(103,588);  //    2
    add_knob(68,429,-5.0,0.0,5.0);

    //control
    add_inlet(68 ,327);   // 3

    add_switch(68,291);
    add_switch(68,364);
}

v201::~v201()
{
    //dtor
}


void v201::dsp(){
    double fade, pan;
    if(switches[0]->get_value()){
        fade = inlets[3]->pull_sample()/5.0;
    }else{
        fade = knobs[0]->get_value()/5.0;
    }
    if(switches[1]->get_value()){
        pan = inlets[3]->pull_sample()/5.0;
    }else{
        pan = knobs[1]->get_value()/5.0;
    }

    double in1 = inlets[0]->pull_sample();
    double in2 = inlets[1]->pull_sample();
    double i = inlets[2]->pull_sample();

    double f1 = (-fade + 1.0)/2.0;
    double f2 = ( fade + 1.0)/2.0;
    double p1 = (-pan  + 1.0)/2.0;
    double p2 = ( pan  + 1.0)/2.0;

    double o  = f1*in1 + f2*in2;
    double o1 = p1*i;
    double o2 = p2*i;

    outlets[0]->push_sample(o);
    outlets[1]->push_sample(o1);
    outlets[2]->push_sample(o2);
}

void v201::draw(const Cairo::RefPtr<Cairo::Context>& cr){
    draw_background(cr,0.5);
    draw_caption_and_id(cr);

    cr->save();
    cr->translate(xoffset,0);

    cr->set_source_rgb(0.7,0.7,0.7);
    cr->set_line_width(2.5);
    cr->set_line_cap(Cairo::LINE_CAP_ROUND);
    /*cr->move_to(33,85);
    cr->line_to(68 ,149);
    cr->line_to(103,85);
    cr->stroke();
    cr->move_to(68 ,149);
    cr->line_to(68 ,524);
    cr->stroke();
    cr->move_to(103,588);
    cr->line_to(68 ,524);
    cr->line_to(33 ,588);
    cr->stroke();
    */

    cr->move_to(68 ,236);
    cr->line_to(68 ,429
    );
    cr->stroke();

    draw_text(cr,"IN 1",10.0, 33,  55);
    draw_text(cr,"IN 2",10.0, 103,  55);
    draw_text(cr,"OUTPUT"  ,10.0, 68,  115);


    draw_text(cr,"INPUT",10.0, 68,  491);
    draw_text(cr,"OUT 1"  ,10.0, 33,  554);
    draw_text(cr,"OUT 2" ,10.0, 103, 554);

    cr->restore();
}
