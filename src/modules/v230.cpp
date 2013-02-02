/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#include "v230.h"

v230::v230()
{
    name = "ADSR envemope";
    caption = "ADSR";
    type_id = 230;
    panel_width = 136;
    add_inlet(33 ,588);
    add_outlet(103,588);
    add_knob(68,110,0.0,2.0,10.0);  //A
    add_knob(68,230,0.0,5.0,10.0);   //D
    add_knob(68,350,0.0,8.0,10.0);   //S
    add_knob(68,470,0.0,7.0,10.0);   //R

    phase = 0;
    current_val = 0.0;
    last_input = 0.0;
    d = 0.0;
}

v230::~v230()
{
    //dtor
}

//converts time specified by knobs (0-10) to speed of signal change per sample
double val2d(double from, double to, double val){
    double ms = pow(1.725034076,val)*15;
    double freq = 1000.0/ms;
    double d = freq/44100.0;
    double diff = to - from;
    return diff*d;
}

void v230::dsp(){
    double input = inlets[0]->pull_sample();
    if(input >= 1.0 + last_input){
        phase = 1;
        d = val2d(current_val, 5.0, knobs[0]->get_value());
    }
    if(input <= - 1.0 + last_input){
        phase = 4; //release
        d = val2d(current_val, 0.0, knobs[3]->get_value());
    }
    last_input = input;

    current_val += d;
    if(phase == 1){ //attack phase
        if (current_val >= 5.0){ //next phase...
            current_val = 5.0;
            if(knobs[2]->get_value() != 10.0){
                phase = 2; //decay
                d = val2d(current_val, knobs[2]->get_value()/2.0, knobs[1]->get_value());
            }else{
                //no decay phase
                phase = 3; //sustain (at max = 1.0)
                d = 0.0;
            }
        }
    }else if(phase == 2){ //decay phase
        if (current_val <= knobs[2]->get_value()/2.0){ //suspend now...
            current_val = knobs[2]->get_value()/2.0;
            d = 0.0;
            phase = 3;
        }
    }if(phase == 4){ //release phase
        if (current_val <= 0.0){ //end envelope...
            current_val = 0.0;
            phase = 0;
            d = 0.0;
        }
    }

    outlets[0]->push_sample(current_val);
}

void v230::draw(const Cairo::RefPtr<Cairo::Context>& cr){
    draw_background(cr,0.3);
    draw_caption_and_id(cr);
    cr->save();
    cr->translate(xoffset,0);
    draw_text(cr,"GATE"  ,10.0, 33,  554);
    draw_text(cr,"ENVELOPE" ,10.0, 103, 554);
    draw_text(cr,"ATTACK TIME",10.0, 68,  45);
    draw_text(cr,"DECAY TIME",10.0, 68, 168);
    draw_text(cr,"SUSTAIN LEVEL",10.0, 68, 288);
    draw_text(cr,"RELEASE TIME",10.0, 68, 408);

    cr->set_source_rgb(1.0,1.0,1.0);
    cr->set_line_width(2.5);
    cr->set_line_cap(Cairo::LINE_CAP_ROUND);
    cr->move_to(12,532);
    cr->line_to(panel_width - 12, 532);
    cr->stroke();

    cr->restore();
}
