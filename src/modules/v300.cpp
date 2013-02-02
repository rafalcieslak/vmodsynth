/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#include "v300.h"
#include "algorithms.h"

v300::v300()
{
    name = "Filter";
    caption = "FILTER";
    type_id = 300;
    panel_width = 215;

    add_knob(144,110,0.0,2.0,10.0); //Frequency
    add_knob(108,230,0.0,0.0, 1.0,10.0); //Bandwidth
    add_knob( 55,350,0.0,0.0,10.0); //Amount1
    add_knob(161,350,0.0,0.0,10.0); //Amount2

    add_inlet ( 36, 513); //input 1
    add_inlet ( 36, 588); //input 2
    add_outlet(108, 513); //lowpass
    add_outlet(108, 588); //highpass
    add_outlet(180, 513); //bandpass
    //add_outlet(180, 588); //notch

    add_inlet ( 45, 128); //1v/oct

    add_inlet ( 55, 425); //freq mod
    add_inlet (161, 425); //band mod

    freq_last = 0.0;
    hip_last = 0.0;
    lop_last = 0.0;
    bp_last = 0.0;
    bp_prev = 0.0;
}

v300::~v300()
{
}

void v300::dsp(){
    double freq = 612.0*pow(24.5,knobs[0]->get_value()/5.0 - 1.0);

    freq *= M_PI/20.0;

    if(inlets[2]->connection){
        freq = 8.17579891564 * exp(0.69314718 * inlets[2]->pull_sample());
        freq *= pow(4.0,knobs[0]->get_value()/5.0 - 1.0);
        //std::cout << pow(2.0,knobs[0]->get_value()/5.0 - 1.0) << std::endl;
    }

    freq *= (1.0 + knobs[2]->get_value()*inlets[3]->pull_sample()/50.0);

    //low-pass filter to smooth freq changes
    freq += freq_last;
    freq_last = freq*(1.0-M_PI/20.0);

    double angle = 2.0*M_PI*freq/44100.0;
    double bandwidth = knobs[1]->get_value();
    bandwidth = 0.01*pow(1000,bandwidth);
    bandwidth *= (1.0 + knobs[3]->get_value()*inlets[4]->pull_sample()/50.0);

    double in = inlets[0]->pull_sample()/5.0 + inlets[1]->pull_sample()/5.0;

    {
        double angle = freq * (2.0 * 3.14159) / 44100.0;
        double p;
        if (bandwidth < 0.001) p = 1.0;
        else p = angle/bandwidth;
        //std::cout << p << " " << angle << " " << bandwidth << std::endl;
        if (p > 1.0) p = 1.0;
        double r = 1.0 - p;
        double coef1 = 2.0 * cos(angle) * r;
        double coef2 = - r * r;
        double gain = 2 * p * (p + r * angle);

        double out = in + coef1*bp_last + coef2*bp_prev;
        bp_prev = bp_last;
        bp_last = out;
        outlets[2]->push_sample(gain*out);
    }

    {
        double coef = freq * (2 * 3.14159) / 44100.0;
        if     (coef < 0.0) coef = 0.0;
        else if(coef > 1.0) coef = 1.0;
        double feedback = 1.0 - coef;

        double out = coef*in + feedback*lop_last;
        lop_last = out;
        outlets[0]->push_sample(out);
    }

    {
        double coef = 1.0 - freq * (2 * 3.14159) / 44100.0;
        if     (coef < 0.0) coef = 0.0;
        else if(coef > 1.0) coef = 1.0;

        if(coef < 1.0){
            double out = in + coef*hip_last;
            outlets[1]->push_sample(out - hip_last);
            hip_last = out;
        }else{
            outlets[1]->push_sample(in);
            hip_last = 0.0;
        }
    }
}

void v300::draw(const Cairo::RefPtr<Cairo::Context>& cr){
    draw_background(cr,0.2);
    draw_caption_and_id(cr);
    cr->save();
    cr->translate(xoffset,0);
    cr->set_source_rgb(1.0,1.0,1.0);
    cr->set_line_width(2.5);
    cr->set_line_cap(Cairo::LineCap::LINE_CAP_ROUND);

    draw_text(cr,"BAND-PASS"    ,10.0, 180, 480);
    draw_text(cr,"LOW-PASS"     ,10.0, 108, 479.0);
    draw_text(cr,"HIGH-PASS"    ,10.0, 108, 554.0);
    draw_text(cr,"SIGNAL"       ,10.0,  36, 472);
    draw_text(cr,"INPUT #1"     ,10.0,  36, 482);
    draw_text(cr,"SIGNAL"       ,10.0,  36, 549);
    draw_text(cr,"INPUT #2"     ,10.0,  36, 559);

    draw_text(cr,"FREQUENCY"    ,10.0, 108,  45);
    draw_text(cr,"1V/OCTAVE"    ,10.0, 45, 95);

    cr->move_to(12,158);
    cr->line_to(panel_width-12,158);
    cr->stroke();
    draw_text(cr,"BANDWIDTH"    ,10.0, 108, 168);

    cr->move_to(12,281);
    cr->line_to(panel_width-12,281);
    cr->stroke();

    draw_text(cr,"FREQ. MODULATION",10.0,  55, 290, 0.7);
    draw_text(cr,"BAND. MODULATION",10.0, 161, 290, 0.7);
    cr->move_to(55,350);
    cr->line_to(55,425);
    cr->stroke();
    cr->move_to(161,350);
    cr->line_to(161,425);
    cr->stroke();

    cr->move_to(12,460);
    cr->line_to(panel_width-12,460);
    cr->stroke();

    cr->restore();
}
