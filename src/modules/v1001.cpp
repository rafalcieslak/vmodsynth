/*
    Copyright (C) 2012, 2013, 2016 Rafał Cieślak

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


#include "v1001.h"
#include "algorithms.h"
#include "../AlsaDriver.h"

v1001::v1001()
{
    name = "Audio out";
    caption = "AUDIO OUT";
    type_id = 1001;
    panel_width = 136;
    add_inlet(68,90); //mono
    add_inlet(33,280); //stereo l
    add_inlet(103,280); //stereo r
    add_knob(68,190,0.0,0.0,100.0,10); //amplitude
    knobs[0]->size = 30.0;
}

v1001::~v1001()
{
    //dtor
}

void v1001::dsp_full(AudioContext ctx){
    double amplitude = db2rms(knobs[0]->get_value());
    if(inlets[1]->connection || inlets[2]->connection){
        //stereo mode
        double l = amplitude*inlets[1]->pull_sample()/5.0;//input range is -5 to 5
        double r = amplitude*inlets[2]->pull_sample()/5.0;//input range is -5 to 5
        ctx.audio_driver->AppendSample(l,r);
    }else{
        //mono mode
        double s = amplitude*inlets[0]->pull_sample()/5.0;//input range is -5 to 5
        ctx.audio_driver->AppendSample(s,s);
    }
}

void v1001::draw(const Cairo::RefPtr<Cairo::Context>& cr){
    draw_background(cr,0.7);
    draw_caption_and_id(cr);

    draw_text(cr,"MONO OUT"   ,10.0,xoffset+68,  56);
    draw_text(cr,"AMPLITUDE (dB)",10.0,xoffset+68,130);
    draw_text(cr,"STEREO OUT",10.0,xoffset+68,246);
}
