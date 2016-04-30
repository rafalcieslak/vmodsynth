/*
    Copyright (C) 2016 Robert Gyllenberg

    extends Module class by Rafał Cieślak 2012, 2013

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


#include "v212.h"

v212::v212()
{
    name = "Fade / Offset";
    caption = "GAIN + OFFSET";
    type_id = 212;
    panel_width = 136;

    add_inlet(33, 338);         //In 0

    add_outlet(33 ,85);         //Out 0
    add_outlet(103 ,85);        //Out 1

    add_inlet(103, 338);        //In 1

    add_outlet(33 ,590);        //Out 2
    add_outlet(103 ,590);       //Out 3

    add_knob(68,160,-5.0,0.0,5.0);
    add_knob(68,260, 0.0,1.0,5.0);

    add_knob(68,420, 0.0,1.0,5.0);
    add_knob(68,520,-5.0,0.0,5.0);

    //add_switch(68,364);
}

v212::~v212()
{
    //dtor
}


void v212::dsp(){
    double gain_a, offset_a, gain_b, offset_b, in_a, in_b, out_a, out_b;

    offset_a = knobs[0]->get_value();
    gain_a = knobs[1]->get_value();
    gain_b = knobs[2]->get_value();
    offset_b = knobs[3]->get_value();

    in_a=inlets[0]->pull_sample();
    in_b=inlets[1]->pull_sample();

    out_a=in_a*gain_a+offset_a;
    out_b=in_b*gain_b+offset_b;

    outlets[0]->push_sample(out_a);
    outlets[1]->push_sample(out_a);
    outlets[2]->push_sample(out_b);
    outlets[3]->push_sample(out_b);
}

void v212::draw(const Cairo::RefPtr<Cairo::Context>& cr){
    draw_background(cr,0.5);
    draw_caption_and_id(cr);

    cr->save();
    cr->translate(xoffset,0);

    cr->set_source_rgb(0.7,0.7,0.7);
    cr->set_line_width(2.5);
    cr->set_line_cap(Cairo::LINE_CAP_ROUND);

    cr->set_line_width(1.5);
    cr->move_to(33 ,338);
    cr->line_to(68 ,303);
    cr->line_to(68 ,85);
    cr->stroke();

    cr->move_to(33 ,85);
    cr->line_to(103 ,85);
    cr->stroke();

    draw_text(cr,"OUT A",12.0, 68,  50);

    draw_text(cr,"IN",12.0, 68,  338);

    draw_text(cr,"1",10.0, 33,  55);
    draw_text(cr,"2",10.0, 103,  55);

    draw_text(cr,"OUT",12.0, 68,  570);
    draw_text(cr,"B",12.0, 68,  590);
    draw_text(cr,"1" ,10.0, 33,  610);
    draw_text(cr,"2" ,10.0, 103, 610);

    cr->restore();
}
