/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#include "v702.h"
#include <cmath>

v702::v702()
{
    name = "Overdrive effect";
    caption = "OVERDRIVE";
    type_id = 702;
    panel_width = 136;
    add_inlet(33 ,588);
    add_outlet(103,588);
    add_knob(68,107,0.0,5.0,10.0);
    add_knob(68,230,0.0,5.0,10.0);
    add_switch(68,300,"ATAN","CLIP",false);
}

v702::~v702()
{
    //dtor
}

void v702::dsp(){
    double in = inlets[0]->pull_sample();
    double gain = knobs[0]->get_value()*2.0;
    double level = knobs[1]->get_value()/5.0;
    if(switches[0]->get_value() == false) //atan
        outlets[0]->push_sample( level*atan(gain*in)*4.0/M_PI );
    else
        outlets[0]->push_sample( level* ((gain*in>5.0)?5.0:(gain*in<-5.0)?-5.0:gain*in));
}

void v702::draw(const Cairo::RefPtr<Cairo::Context>& cr){
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

    draw_text(cr,"GAIN",10.0, 68,  45);
    draw_text(cr,"LEVEL",10.0, 68, 168);
    draw_text(cr,"INPUT"  ,10.0, 33,  554);
    draw_text(cr,"OUTPUT" ,10.0, 103, 554);

    cr->restore();
}
