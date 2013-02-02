/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#include "v101.h"
v101::v101()
{
    name = "Noise source";
    caption = "NOISE SOURCE";
    type_id = 203;
    panel_width = 136;
    add_outlet(33,90);   //white1
    add_outlet(103,90);  //white2
    add_outlet(33,170);   //pink1
    add_outlet(103,170);  //pink2
    b0 = b1 = b2 = b3 = b4 = b5 = b6 = 0.0;
}

v101::~v101()
{
    //dtor
}

void v101::dsp(){
    double s = 0.0;
    for(int x = 0; x < 64; x++){
        s += ((rand()%128)/127.0); // 0.0 - 1.0
    } //s will be ranging 0 - 64;
    s /= 64.0;
    s -= 0.5;
    s *= 5.0;
    double white = s;
    outlets[0]->push_sample(white);
    outlets[1]->push_sample(white);

    //Paramaters based on values proposed by Paul Kellet
    b0 = 0.99886 * b0 + white * 0.0555179;
    b1 = 0.99332 * b1 + white * 0.0750759;
    b2 = 0.96900 * b2 + white * 0.1538520;
    b3 = 0.86650 * b3 + white * 0.3104856;
    b4 = 0.55000 * b4 + white * 0.5329522;
    b5 = -0.7616 * b5 - white * 0.0168980;
    double pink = b0 + b1 + b2 + b3 + b4 + b5 + b6 + white * 0.5362;
    b6 = white * 0.115926;

    pink *= 0.3;
    outlets[2]->push_sample(pink);
    outlets[3]->push_sample(pink);
}

void v101::draw(const Cairo::RefPtr<Cairo::Context>& cr){
    draw_background(cr,0.3);
    draw_caption_and_id(cr);

    cr->save();
    cr->translate(xoffset,0);

    cr->set_source_rgb(1.0,1.0,1.0);
    cr->set_line_width(3.0);
    cr->move_to(33,90);
    cr->line_to(103,90);
    cr->stroke();
    cr->move_to(33,170);
    cr->line_to(103,170);
    cr->stroke();

    draw_text(cr,"WHITE NOISE"   ,10.0, 68,  56);
    draw_text(cr,"PINK NOISE"   ,10.0, 68,  136);

    cr->restore();
}
