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


#include <iostream>
#include <string>
#include <sstream>
#include "Knob.h"
#include "Module.h"
#include "MainWindow.h"
#include "Engine.h"

extern MainWindow* mainwindow;

Knob::Knob(Module* _parent, int _x, int _y, double _min, double _value, double _max, int _steps)
{
    subtype = 0; //Basic Knob unless proved otherwise RG2016
    parent = _parent;

    x = _x;
    y = _y;

    min_val = _min;
    max_val = _max;
    value = _value;
    if(_steps)
        steps = _steps;
    else
        steps = (int)(_max - _min);
    if(steps > 20) steps = 20;
    range = (max_val-min_val);

    controlled = false;

    size = 35.0;

    redraw_me.connect(sigc::mem_fun(*this,&Knob::redraw_me_real));
}

Knob::~Knob()
{
    //dtor
}

double Knob::get_value(){
    return value;
}

void Knob::set_value(double val) //RG 2016
{
 //std::cerr << "Hello from Knob::set_value() \n";
 value = val;
 redraw_me();
}


void Knob::movement_start(){
    //std::cout << "Knob movement started.\n";
    movement_value = value;
}

void Knob::movement_position(int x, int y){
    //std::cout << "Knob moved by "<<x<<" "<<y<<".\n";
    if ((x*x > size*size || y*y > size*size) && x*x > y*y) y = 0;
    double newval = movement_value - (double)y*range/200.0 + (double)x*range/2000.0;
    value = std::min(std::max(newval,min_val),max_val);
    redraw_me();
}

void Knob::movement_end(){
    //std::cout << "Knob movement ended.\n";
}

bool Knob::is_point_within(double _x, double _y){
    _x -= parent->xoffset + x;
    _y -= y;
    _x = _x*_x;
    _y = _y*_y;
    double r = _x+_y;
    if(r<size*size)
        return true;
    else
        return false;
}

void Knob::set_value_from_controller(int v){
    double v2 = v/127.0;
    value = min_val + range*v2;
    redraw_me();
}

void Knob::redraw_me_real(){
    double scale = Engine::get_gui_scale();
    mainwindow->cabinet.queue_draw_area((parent->xoffset + x - size)*scale, (y - size)*scale, (2*size)*scale,(2*size)*scale);
}

void Knob::draw(const Cairo::RefPtr<Cairo::Context>& cr){


    cr->save();
    cr->translate(parent->xoffset + x, y);

    //silhouette
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->arc(0,0, size, 0.0, 2*M_PI);
    cr->fill();

    //marker
    double angle = 2*M_PI*(value - min_val)/range;
    angle *= 0.8;
    angle += 0.6*2*M_PI;
    cr->save();
    cr->rotate(angle);
    cr->set_source_rgb(1.0, 1.0, 1.0);
    cr->set_line_width(3.0);
    cr->move_to(0,-size);
    cr->line_to(0,-size*0.6);
    cr->stroke();
    cr->restore();

    //value points
    cr->save();
    cr->set_source_rgb(1.0, 1.0, 1.0);
    cr->set_line_width(1.5);
    cr->rotate(0.6*2*M_PI);
    for(int p = 0; p <= steps; p++){
        cr->save();
        //marker
        cr->rotate(p*0.8*2*M_PI/(double)steps);
        cr->move_to(0,-size);
        cr->line_to(0,-size*1.15);
        cr->stroke();
        //value text
        double val = min_val + p*range/steps;
        std::stringstream ss; ss << val; std::string text; ss >> text;
        cr->translate(0,-size*1.15 - 8);
        cr->rotate(-p*0.8*2*M_PI/(double)steps);
        cr->rotate(-0.6*2*M_PI);
        cr->select_font_face("Verdana",Cairo::FONT_SLANT_NORMAL,Cairo::FONT_WEIGHT_BOLD);
        cr->set_font_size(9.0);
        Cairo::TextExtents tx;
        cr->get_text_extents(text,tx);
        cr->set_source_rgb(1.0,1.0,1.0);
        cr->move_to(-tx.width/2 , tx.height/2 );
        cr->show_text(text);
        cr->restore();
    }
    cr->restore();

    //inner circle
    if(controlled)
        cr->set_source_rgb(0.7, 0.3, 0.3);
    else
        cr->set_source_rgb(1.0, 1.0, 1.0);
    cr->arc(0,0, size*0.62, 0.0, 2*M_PI);
    cr->fill();

    //value
    char val[10];
    sprintf(val,"%.03lf",value);
    cr->set_font_size(10.0);
    Cairo::TextExtents tx;
    cr->get_text_extents(val,tx);
    cr->set_source_rgb(0.0,0.0,0.0);
    cr->move_to(- tx.width/2,tx.height/2);
    cr->show_text(val);

    if(controlled){
        //controller data
        sprintf(val,"(%d: %d)",channel, controller);
        cr->set_font_size(10.0);
        Cairo::TextExtents tx;
        cr->get_text_extents(val,tx);
        cr->set_source_rgb(1.0,1.0,1.0);
        cr->move_to(- tx.width/2,12.0 + tx.height/2);
        cr->show_text(val);
    }
    cr->restore();
}
