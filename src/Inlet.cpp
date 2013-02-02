/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#include "Inlet.h"
#include "Module.h"
#include "Wire.h"

Inlet::Inlet(Module* _parent, int _x, int _y)
{
    parent = _parent;
    x = _x;
    y = _y;
    connection = NULL;
    highlight = false;
}

Inlet::~Inlet()
{
    //dtor
}

double Inlet::pull_sample(){
    if(!connection) return 0.0;
    else return connection->out_sample;
}

bool Inlet::is_point_within(double _x, double _y){
    _x -= parent->xoffset + x;
    _y -= y;
    _x = _x*_x;
    _y = _y*_y;
    double r = _x+_y;
    if(r<24*24)
        return true;
    else
        return false;
}

void Inlet::draw(const Cairo::RefPtr<Cairo::Context>& cr){
    cr->save();
    cr->translate(parent->xoffset + x, y);
    //outer (highlightable) circle
    if(highlight)
        cr->set_source_rgb(100.0/256.0,100.0/256.0, 1.0);
    else
        cr->set_source_rgb(200.0/256.0, 200.0/256.0, 215.0/256.0);
    cr->arc(0,0, 24, 0.0, 2*M_PI);
    cr->fill();
    //outer circle base
    cr->set_source_rgb(200.0/256.0, 200.0/256.0, 215.0/256.0);
    cr->arc(0,0, 24-3.0, 0.0, 2*M_PI);
    cr->fill();
    //hexagon
    cr->set_source_rgb(140.0/256.0, 140.0/256.0, 150.0/256.0);
    cr->move_to(-10.5,18.1);
    cr->line_to(10.5,18.1);
    cr->line_to(20.9,0.0);
    cr->line_to(10.5,-18.1);
    cr->line_to(-10.5,-18.1);
    cr->line_to(-20.9,0.0);
    cr->close_path();
    cr->fill();

    cr->save();
    cr->scale(0.9,0.9);
    cr->set_source_rgb(160.0/256.0, 160.0/256.0, 170.0/256.0);
    cr->move_to(-10.5,18.1);
    cr->line_to(10.5,18.1);
    cr->line_to(20.9,0.0);
    cr->line_to(10.5,-18.1);
    cr->line_to(-10.5,-18.1);
    cr->line_to(-20.9,0.0);
    cr->close_path();
    cr->fill();
    cr->restore();

    //hole
    cr->set_source_rgb(20.0/256.0, 60.0/256.0, 20.0/256.0);
    cr->arc(0,0, 10, 0.0, 2*M_PI);
    cr->fill();
    cr->restore();
}
