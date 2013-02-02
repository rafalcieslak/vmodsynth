/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#include "v400.h"

v400::v400()
{
    name = "2ch mixer";
    caption = "MIXER";
    type_id = 400;
    panel_width = 136;
    add_inlet(33 ,513);
    add_inlet(103,513);
    add_outlet(33 ,588);
    add_outlet(103,588);
    add_knob(68,107,0.0,10.0,10.0);
    add_knob(68,230,0.0,0.0,10.0);
    add_knob(68,352,-5.0,0.0,5.0,10);
}

v400::~v400()
{
    //dtor
}

void v400::dsp(){
    double in1 = inlets[0]->pull_sample();
    double in1a = knobs[0]->get_value()/10.0;
    double in2 = inlets[1]->pull_sample();
    double in2a = knobs[1]->get_value()/10.0;
    double sum = in1*in1a+in2*in2a+knobs[2]->get_value();
    outlets[0]->push_sample(sum);
    outlets[1]->push_sample(-sum);
}

void v400::draw(const Cairo::RefPtr<Cairo::Context>& cr){
    draw_background(cr,0.5);
    draw_caption_and_id(cr);

    cr->save();
    cr->translate(xoffset,0);

    cr->set_source_rgb(1.0,1.0,1.0);
    cr->set_line_width(2.5);
    cr->set_line_cap(Cairo::LINE_CAP_ROUND);
    cr->move_to(12,470);
    cr->line_to(panel_width - 12, 470);
    cr->stroke();

    draw_text(cr,"INPUT #1",10.0, 33,  482);
    draw_text(cr,"INPUT #1 LEVEL",10.0, 68,  45);
    draw_text(cr,"INPUT #2",10.0, 103, 482);
    draw_text(cr,"INPUT #2 LEVEL",10.0, 68, 168);
    draw_text(cr,"OFFSET",10.0, 68, 290);
    draw_text(cr,"OUTPUT"  ,10.0, 33,  554);
    draw_text(cr,"-OUTPUT" ,10.0, 103, 554);

    cr->restore();
}
