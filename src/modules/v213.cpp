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


#include "v213.h"
#include <stdlib.h>

v213::v213()
{
    name = "Random Note";
    caption = "RANDOM NOTE";
    type_id = 213;
    panel_width = 136;

    add_outlet(33 ,85);         //Out 0
    add_outlet(103 ,85);         //Out 1
 
    add_knob(68,160, 0.0,2.0,5.0); //Offset
    add_knob(68,280, 0.5,2.0,4.5); //Range

    add_selector(68,400,5,1);      //Tonality 5, 7, 12 or linear scale
    //add_knob(68,420, 0.0,1.0,5.0);
    add_knob(68,520, 1.0,1.0,5.0); //Tempo

    //add_switch(68,364);

    cnt_time     =  50000;

    last_voltage =  0.0;

    int a[5] = { 0,2,5,7,9 }; //Pentatonical scale
    scale_5.insert(scale_5.end(), a, a+5);

    int b[7] = { 0,2,4,5,7,9,11}; //Diatonical scale
    scale_7.insert(scale_7.end(), b, b+7);
  
    last_note = 0;
 }

v213::~v213()
{
    //dtor
}

double v213::fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void v213::dsp(){
    double offset, octaves, voltage, out = 0.0;
    int note, semitone, scale, range = 0;

    offset   = knobs[0]->get_value();
    octaves  = knobs[1]->get_value();

    cnt_time--;
    if(cnt_time==0)
    { 
        cnt_time = (int)(55000-(knobs[3]->get_value()*10000));

        int scale_knob = knobs[2]->get_value();
        octaves = knobs[1]->get_value();

        switch(scale_knob)
        {
            case 0:
                scale=1;
                range = (int)octaves;
                break;

            case 1:
                scale=5;
                range = (int)(octaves * 6);
                break;

            case 2:
                scale=7;
                range = (int)(octaves * 8);
                break;

            case 3:
                scale=12;
                range = (int)(octaves * 12);
                break;

            case 4:
                scale=999;
                break;
        }


        switch(scale)
        {
            case 1:
                note = (int)(rand() % range);
                semitone  = note*12;
                voltage = offset+semitone/12.0;
                break;

            case 5:
                note = (int)(rand() % range);
                semitone  = (int)(note/5)*12 + scale_5[note % 5];
                voltage = offset+semitone/12.0;
                break;

            case 7:
                note = (int)(rand() % range);
                semitone  = (int)(note/7)*12 + scale_7[note % 7];
                voltage = offset+semitone/12.0;
                break;

            case 12:
                note = (int)(rand() % range);
                semitone  = note; //One to one correspondence here
                voltage = offset+semitone/12.0;
                break;

            case 999: //No distinct tones, but linear voltage
                voltage = fRand(offset, offset+octaves);
                break;

        }
        out = voltage;
        last_voltage = out;
    }
    
    outlets[0]->push_sample(last_voltage);
    outlets[1]->push_sample(last_voltage);
}

void v213::draw(const Cairo::RefPtr<Cairo::Context>& cr){
    draw_background(cr,0.5);
    draw_caption_and_id(cr);

    cr->save();
    cr->translate(xoffset,0);

    cr->set_source_rgb(0.7,0.7,0.7);
    cr->set_line_width(2.5);
    cr->set_line_cap(Cairo::LINE_CAP_ROUND);

    cr->set_line_width(1.5);

    cr->move_to(68 ,160);
    cr->line_to(68 ,85);
    cr->stroke();

    cr->move_to(33 ,85);
    cr->line_to(103 ,85);
    cr->stroke();

    draw_text(cr,"OUT",10.0, 68,  50);

    draw_text(cr,"Offset",10.0, 68,  210);
    draw_text(cr,"Range", 10.0, 68,  330);
    draw_text(cr,"Scale", 10.0, 68,  450);
    draw_text(cr,"1",10.0, 20,  400);
    draw_text(cr,"5",10.0, 35,  365);
    draw_text(cr,"7",10.0, 68,  353);
    draw_text(cr,"12",10.0, (136-35),  365);
    draw_text(cr,"Lin.",10.0, (136-20), 400);

    draw_text(cr,"Tempo",10.0, 68,  570);

    cr->restore();
}
