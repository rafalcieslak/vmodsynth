/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#include "v210.h"
#include "algorithms.h"

v210::v210()
{
    name = "Amplifier";
    caption = "AMPLIFIER";
    type_id = 210;
    panel_width = 136;
    add_inlet(33 ,438); //control 1
    add_inlet(103,438); //control 2
    add_inlet(33 ,513); //input 1
    add_inlet(103,513); //input 2
    add_outlet(33 ,588); //output +
    add_outlet(103,588); //output -
    add_knob(68,107,0.0,5.0,10.0); // initial gain
    add_knob(68,230,0.0,0.0,10.0); // control #1 level
    add_selector(68,330,3,1); // mode
}

v210::~v210()
{
    //dtor
}

void v210::dsp(){
    double in1 = inlets[2]->pull_sample();
    double in2 = inlets[3]->pull_sample();
    double multiplier = knobs[0]->get_value()/2.0
                      + knobs[1]->get_value()/10.0 * inlets[0]->pull_sample()
                      +                         inlets[1]->pull_sample()/5.0;

    int mode = knobs[2]->get_value();
    if(mode == 0){
        multiplier = db2rms(20*multiplier);      //exponential 0-5  ->  0-1   (2.5 -> something small)
    }else if(mode == 1){
        multiplier = multiplier*multiplier/25.0; //square      0-5  ->  0-1   (2.5 -> 0.25)
    }else if(mode == 2){
        multiplier = multiplier/5.0;             //linear      0-5  ->  0-1   (2.5 -> 0.5)
    }
    if(multiplier >  1.0) multiplier =  1.0;
    if(multiplier < -1.0) multiplier = -1.0;
    double out = multiplier*(in1+in2);
    outlets[0]->push_sample( out);
    outlets[1]->push_sample(-out);
}

void v210::draw(const Cairo::RefPtr<Cairo::Context>& cr){
    draw_background(cr,0.3);
    draw_caption_and_id(cr);
    cr->save();
    cr->translate(xoffset,0);
    draw_text(cr,"OUTPUT"  ,10.0, 33,  554);
    draw_text(cr,"-OUTPUT" ,10.0, 103, 554);
    draw_text(cr,"CONTROL" ,10.0, 33,398);
    draw_text(cr,"INPUT #1" ,10.0, 33,408);
    draw_text(cr,"CONTROL" ,10.0, 103, 398);
    draw_text(cr,"INPUT #2" ,10.0, 103, 408);
    draw_text(cr,"SIGNAL" ,10.0, 33, 472);
    draw_text(cr,"INPUT #1" ,10.0, 33, 482);
    draw_text(cr,"SIGNAL" ,10.0, 103, 472);
    draw_text(cr,"INPUT #2" ,10.0, 103, 482);
    draw_text(cr,"INITIAL GAIN",10.0, 68,  45);
    draw_text(cr,"CONTROL #1 LEVEL",10.0, 68, 168);

    cr->set_source_rgb(1.0,1.0,1.0);
    cr->set_line_width(2.5);
    cr->set_line_cap(Cairo::LINE_CAP_ROUND);
    cr->move_to(12,383);
    cr->line_to(panel_width - 12, 383);
    cr->stroke();

    cr->save();
    cr->translate(22,331);
    cr->rotate(-M_PI_2);
    draw_text(cr,"EXPONENTIAL",10.0,0,0,0.9);
    cr->rotate(M_PI_2);
    cr->translate(45,-45);
    draw_text(cr,"SQUARE",10.0,0,0);
    cr->translate(44,45);
    cr->rotate(M_PI_2);
    draw_text(cr,"LINEAR",10.0,0,0);
    cr->restore();

    cr->restore();
}
