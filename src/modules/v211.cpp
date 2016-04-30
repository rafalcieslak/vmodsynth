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


#include "v211.h"

v211::v211()
{
    name = "Summing";
    caption = "SUM";
    type_id = 211;
    panel_width = 68;

    //summing point
    add_inlet(36,85);     // 0
    add_inlet(36,153);    // 1
    add_inlet(36,221);    // 2
    add_inlet(36,289);    // 3

    add_outlet(36 ,450);  // 0
    add_outlet(36 ,518);  // 1
    add_outlet(36 ,586);  // 2
}

v211::~v211()
{
    //dtor
}

void v211::dsp(){

    double in1 = inlets[0]->pull_sample();
    double in2 = inlets[1]->pull_sample();
    double in3 = inlets[2]->pull_sample();
    double in4 = inlets[3]->pull_sample();

    double sum = in1/4.0+in2/4.0+in3/4.0+in4/4.0;
    outlets[0]->push_sample(sum);
    outlets[1]->push_sample(sum);
    outlets[2]->push_sample(sum);
}

void v211::draw(const Cairo::RefPtr<Cairo::Context>& cr){
    draw_background(cr,0.5);
    draw_caption_and_id(cr);

    cr->save();
    cr->translate(xoffset,0);

    cr->set_source_rgb(0.7,0.7,0.7);
    cr->set_line_width(2.5);
    cr->set_line_cap(Cairo::LINE_CAP_ROUND);

    draw_text(cr,"IN",10.0, 33,  55);
    draw_text(cr,"OUT"  ,10.0, 33,  400);
    draw_text(cr,"-12dB" ,10.0, 33, 412);

    cr->restore();
}
