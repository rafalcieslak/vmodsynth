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


#include <string>
#include <sstream>
#include "Module.h"
#include "Inlet.h"
#include "Outlet.h"
#include "Engine.h"
#include "Selector.h"
#include "Switch.h"

Module::Module()
{
    name = "";
    caption = "";
    xoffset = 0;
    highlight = false;
}

Module::~Module()
{
    for(auto &i : inlets){
        Engine::disconnect(i->connection);
        Engine::unregister_inlet(i);
        delete i;
    }
    for(unsigned int i = 0; i < outlets.size(); i++){
        Engine::disconnect(outlets[i]->connection);
        Engine::unregister_outlet(outlets[i]);
        delete outlets[i];
    }
    for(unsigned int i = 0; i < knobs.size(); i++){
        Engine::unregister_knob(knobs[i]);
        delete knobs[i];
    }
    for(unsigned int i = 0; i < switches.size(); i++){
        Engine::unregister_switch(switches[i]);
        delete switches[i];
    }
}

Module::Module(const Module&)
{
    //copy ctor
}

bool Module::is_point_within(double x, double){
    return (x >= xoffset && x <= xoffset + panel_width);
}

int Module::add_inlet(int x, int y){
    Inlet* newinlet = new Inlet(this,x,y);
    inlets.push_back(newinlet);
    Engine::register_inlet(newinlet);
}

int Module::add_outlet(int x, int y){
    Outlet* newoutlet = new Outlet(this,x,y);
    outlets.push_back(newoutlet);
    Engine::register_outlet(newoutlet);
}

int Module::add_knob(int x, int y, double _min, double _value, double _max, int steps){
    Knob* newknob = new Knob(this,x,y,_min,_value,_max,steps);
    knobs.push_back(newknob);
    Engine::register_knob(newknob);
}

int Module::add_selector(int x, int y, int vals, int def, bool hide){
    Selector* newknob = new Selector(this,x,y,vals,def,hide);
    knobs.push_back(newknob);
    Engine::register_knob(newknob);
}

int Module::add_switch(int x, int y, std::string text1, std::string text2, bool vertical, bool val){
    Switch* newswitch = new Switch(this,x,y,text1,text2,vertical,val);
    switches.push_back(newswitch);
    Engine::register_switch(newswitch);
}

void Module::draw_background(const Cairo::RefPtr<Cairo::Context>& cr, double){
    cr->save();
    cr->set_source_rgb(60.0/256.0,60.0/256.0,65.0/256.0);
    cr->move_to(xoffset                 + 1.5,                       1.5);
    cr->line_to(xoffset                 + 1.5, MODULE_PANEL_HEIGHT - 1.5);
    cr->line_to(xoffset + panel_width   + 0.5, MODULE_PANEL_HEIGHT - 1.5);
    cr->line_to(xoffset + panel_width   + 0.5,                       1.5);
    cr->close_path();
    cr->fill_preserve();
    if(highlight){
        cr->set_line_width(3.0);
        cr->set_source_rgb(0.4, 0.4, 0.8);

    }else{
        cr->set_line_width(1.0);
        cr->set_source_rgb(0.0, 0.0, 0.0);
    }
    cr->stroke();
    cr->restore();
}

void Module::draw_caption_and_id(const Cairo::RefPtr<Cairo::Context>& cr){
    cr->save();
    cr->move_to(xoffset+5.0,12.0);
    cr->select_font_face("Verdana",Cairo::FONT_SLANT_NORMAL,Cairo::FONT_WEIGHT_BOLD);
    std::stringstream ss; ss << type_id; std::string tid; ss >> tid;
    cr->set_source_rgb(0.6,0.6,0.6);
    cr->show_text("v"+tid);
    Cairo::TextExtents tx;
    cr->set_font_size(14.0);
    cr->get_text_extents(caption,tx);
    //cr->set_source_rgb(0.4,0.4,0.4);
    //cr->move_to(xoffset+panel_width/2 - tx.width/2 - 2.0,28.0);
    //cr->show_text(caption);
    cr->set_source_rgb(1.0,1.0,1.0);
    cr->move_to(xoffset+panel_width/2.0 - tx.width/2.0 ,     30.0);
    cr->show_text(caption);
    cr->restore();
}

void Module::draw_text(const Cairo::RefPtr<Cairo::Context>& cr, const char *text, double size, double x, double y, double xscale, double r, double g, double b){
    cr->save();
    cr->translate(x,y);
    cr->scale(xscale,1.0);
    cr->select_font_face("Verdana",Cairo::FONT_SLANT_NORMAL,Cairo::FONT_WEIGHT_BOLD);
    cr->set_font_size(size);
    Cairo::TextExtents tx;
    cr->get_text_extents(text,tx);
    cr->set_source_rgb(r,g,b);
    cr->move_to(-tx.width/2 , tx.height/2 );
    cr->show_text(text);
    cr->restore();
}
