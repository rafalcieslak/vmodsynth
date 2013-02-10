/*
    Copyright (C) 2012, 2013 Rafał Cieślak

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


#include "v703.h"
#include <cmath>

v703::v703()
{
    name = "Reverb";
    caption = "REVERBATOR";
    type_id = 703;
    panel_width = 136;
    add_inlet(68 ,513);
    add_outlet(33 ,588);
    add_outlet(103,588);
    add_knob(68,107,0.0,3.333,10.0); //roomsize
    add_knob(68,230,0.0,8.5,10.0); //damp
    add_knob(68,350,-5.0,0.0,5.0); // D/W
}

v703::~v703()
{
    //dtor
}

void v703::dsp(){
    double orig = inlets[0]->pull_sample();
    double in = orig;

    //early echoes
    e1.push(in); //11 ms
    while(e1.size() > 418)  e1.pop();
    if(e1.size() ==   418) {in += e1.front(); e1.pop();}
    e2.push(in); //17 ms
    while(e2.size() > 750)  e2.pop();
    if(e2.size() ==   750) {in += e2.front(); e2.pop();}
    e3.push(in); //22 ms
    while(e3.size() > 970)  e3.pop();
    if(e3.size() ==   970) {in += e3.front(); e3.pop();}

    double roomsize = knobs[0]->get_value()/3.0;

    double s1 = 0.0, s2 = 0.0, s3 = 0.0, s4 = 0.0;
    double time1 = 52.1 * roomsize, time2 = 93.4 * roomsize, time3 = 84.1 * roomsize, time4 = 69.5 * roomsize;
    int d1 = 44.1000*time1, d2 = 44.1000*time2, d3 = 44.1000*time3, d4 = 44.1000*time4;

    //read from main reverbating delay tables

    //missync?
    while(m1.size() > d1) m1.pop();
    while(m2.size() > d2) m2.pop();
    while(m3.size() > d3) m3.pop();
    while(m4.size() > d4) m4.pop();
    if(m1.size() == d1) {s1 += m1.front(); m1.pop();}
    if(m2.size() == d2) {s2 += m2.front(); m2.pop();}
    if(m3.size() == d3) {s3 += m3.front(); m3.pop();}
    if(m4.size() == d4) {s4 += m4.front(); m4.pop();}

    s1 += in;
    s2 += in;

    //rotate the signal
    double angle1 = 3.0*M_PI/8.0;
    double angle2 = M_PI/4.0;
    double a = cos(angle1), b = sin(angle1);
    double c = cos(angle2), d = sin(angle2);
    double p1 = a*s1+b*s3, p3 = -b*s1+a*s3;
    double p2 = a*s2+b*s4, p4 = -b*s2+a*s4;
    double q1 = c*p1+d*p2, q2 = -d*p1+c*p2;
    double q3 = c*p3+d*p4, q4 = -d*p3+c*p4;

    //apply dampening
    double dampening = knobs[1]->get_value()/10.0;
    q1*=dampening;q2*=dampening;q3*=dampening;q4*=dampening;

    //write to output
    double wet =    knobs[2]->get_value()/10.0 + 0.5;
    double dry =  - knobs[2]->get_value()/10.0 + 0.5;
    //std::cout << wet << " " << dry << std::endl;
    outlets[0]->push_sample(orig*dry + q1*wet);
    outlets[1]->push_sample(orig*dry + q2*wet);

    //write to main reverbating delay tables
    m1.push(q1);m2.push(q2);m3.push(q3);m4.push(q4);
}

void v703::draw(const Cairo::RefPtr<Cairo::Context>& cr){
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

    draw_text(cr,"ROOM SIZE",10.0, 68,  45);
    draw_text(cr,"DAMPENING",10.0, 68, 168);
    draw_text(cr,"DRY/WET",10.0, 68, 288);
    draw_text(cr,"INPUT",10.0, 68,  482);
    draw_text(cr,"OUTPUT L"  ,10.0, 33,  554);
    draw_text(cr,"OUTPUT R" ,10.0, 103, 554);

    cr->restore();
}
