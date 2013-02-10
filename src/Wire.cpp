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

#include "Wire.h"
#include "Inlet.h"
#include "Outlet.h"
#include "Module.h"

#include <iostream>

const double B = 0.4*M_PI; //wire's arc radius distance angle ( range: (0, pi), the smaller, the greater arc's radius
const double B2 = (2.0*cos(B));

Wire::Wire(Outlet* _from, Inlet* _to)
{
    from = _from;
    to = _to;
    in_sample = 0.0;
    out_sample = 0.0;
    double hue = ((double)(rand()%100))/100.0;
    g_delta = 0.6*hue;
    b_delta = 0.6*(1.0-hue);
}

Wire::~Wire()
{
    //dtor
}

void Wire::pass_sample(){
    out_sample = in_sample;
    in_sample = 0.0;
}

void Wire::draw(const Cairo::RefPtr<Cairo::Context>& cr){
    cr->save();
    cr->set_line_width(4.0);
    cr->set_line_cap(Cairo::LINE_CAP_ROUND);
    cr->set_source_rgb(0.1,0.4 + g_delta,0.2 +b_delta);
    int startx = from->parent->xoffset + from->x;
    int starty = from->y;
    int endx = to->parent->xoffset + to->x;
    int endy = to->y;
    double dx = endx - startx;
    double dy = endy - starty;
    double A = atan2(dy,dx) + M_PI;
    double r = sqrt(dx*dx + dy*dy);
    double angle = A+B;
    double l = r/B2;
    double nx = l*cos(angle);
    double ny = l*sin(angle);
    double Ox = startx - nx;
    double Oy = starty - ny;
    double Oangle = M_PI - 2*B;
    double Ostart = angle;// + M_PI/2.0;

    cr->move_to(startx,starty);
    cr->arc(Ox,Oy,l,Ostart,Ostart+Oangle);
    cr->stroke();
    /* Arrow:
    double x1 = endx + 15.0 * cos(angle - 0.5);
    double x2 = endx + 15.0 * cos(angle + 0.5);
    double y1 = endy + 15.0 * sin(angle - 0.5);
    double y2 = endy + 15.0 * sin(angle + 0.5);
    cr->move_to(startx,starty);
    cr->line_to(endx,endy);
    cr->stroke();
    cr->move_to(endx,endy);
    cr->line_to(x1,y1);
    cr->stroke();
    cr->move_to(endx,endy);
    cr->line_to(x2,y2);
    cr->stroke();
    */
    cr->restore();
}
