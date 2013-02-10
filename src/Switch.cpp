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


#include "Switch.h"
#include "Module.h"
#include "Engine.h"
#include "MainWindow.h"

extern MainWindow* mainwindow;

Switch::Switch(Module* _parent, int _x, int _y, std::string _text1, std::string _text2, bool ver, bool val)
{
    parent = _parent;
    x = _x;
    y = _y;
    text1 = _text1;
    text2 = _text2;
    value = val;
    vertical = ver;

    redraw_me.connect(sigc::mem_fun(*this,&Switch::redraw_me_real));
}

Switch::~Switch()
{
    //dtor
}

bool Switch::get_value(){
    return value;
}
void Switch::set_value(bool v){
    value = v;
    redraw_me();
}

bool Switch::is_point_within(double _x, double _y){
    _x -= parent->xoffset + x;
    _y -= y;
    _x = _x*_x;
    _y = _y*_y;
    double r = _x+_y;
    if(r<15*15)
        return true;
    else
        return false;
}

void Switch::redraw_me_real(){
    double scale = Engine::get_gui_scale();
    double size = 20.0;
    mainwindow->cabinet.queue_draw_area((parent->xoffset + x - size)*scale, (y - size)*scale, (2*size)*scale,(2*size)*scale);
}

void Switch::draw(const Cairo::RefPtr<Cairo::Context>& cr){

    cr->save();
    cr->translate(parent->xoffset + x, y);
    /*
    cr->set_source_rgb(200.0/256.0, 200.0/256.0, 215.0/256.0);
    cr->set_source_rgb(140.0/256.0, 140.0/256.0, 150.0/256.0);
    cr->set_source_rgb(160.0/256.0, 160.0/256.0, 170.0/256.0);
    */
    cr->set_source_rgb(200.0/256.0, 200.0/256.0, 215.0/256.0);
    cr->arc(0,0, 10, 0.0, 2*M_PI);
    cr->fill();
    //hole
    cr->set_source_rgb(30.0/256.0, 30.0/256.0, 30.0/256.0);
    cr->arc(0,0, 7, 0.0, 2*M_PI);
    cr->fill();

    //handle
    const double len = 15.0;
    double endx, endy;
    if(!vertical){
        if(value) {endx =  len; endy = 0.0;}
        else      {endx = -len; endy = 0.0;}
    }else{
        if(value) {endx = 0.0; endy =  len;}
        else      {endx = 0.0; endy = -len;}
    }
    cr->set_line_width(6.0);
    cr->set_line_cap(Cairo::LINE_CAP_ROUND);
    cr->set_source_rgb(1.0,1.0,1.0);
    cr->move_to(0.0,0.0);
    cr->line_to(endx,endy);
    cr->stroke();
    cr->set_source_rgb(170.0/256.0, 170.0/256.0, 170.0/256.0);
    cr->move_to(endx,endy);
    cr->arc(endx,endy,4,0.0,2*M_PI);
    cr->fill();

    //text
    cr->select_font_face("Verdana",Cairo::FONT_SLANT_NORMAL,Cairo::FONT_WEIGHT_BOLD);
    cr->set_font_size(10.0);
    cr->set_source_rgb(1.0,1.0,1.0);
    Cairo::TextExtents tx;
    if(!vertical){
        cr->get_text_extents(text1,tx);
        cr->move_to(-tx.width - 24.0, 3.0 );
        cr->show_text(text1);
        cr->get_text_extents(text2,tx);
        cr->move_to(22.0, 3.0 );
        cr->show_text(text2);
    }else{
        cr->get_text_extents(text1,tx);
        cr->move_to(-tx.width/2.0, -22.0 );
        cr->show_text(text1);
        cr->move_to(-tx.width/2.0, 27.0 );
        cr->get_text_extents(text2,tx);
        cr->show_text(text2);
    }

    cr->restore();
}
