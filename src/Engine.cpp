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
#include <set>

#define ENGINE_DEFS
#include "Engine.h"
#undef ENGINE_DEFS

#include "Module.h"
#include "ModuleList.h"
#include "Wire.h"

#include "MainWindow.h"
extern MainWindow *mainwindow;

namespace Engine{

Inlet* selected_inlet = NULL;
Outlet* selected_outlet = NULL;
Module* selected_module = NULL;

int last_x_offset = 0;

double gui_scale = 0.8;

// ====== ADDING, REMOVING and MOVING modules ============

void create_and_append_module(int ID){
    Module* newmod = create_new_module_from_ID(ID);
    if (newmod == NULL) //failed to create
        return;
    modules.push_back(newmod);
    newmod->xoffset = last_x_offset;
    last_x_offset += newmod->panel_width + 1;
    mainwindow->cabinet.set_size_request((last_x_offset + 1)*gui_scale,-1);
    mainwindow->cabinet_scrolledwindow.set_size_request(-1,MODULE_PANEL_HEIGHT*gui_scale);
}

std::vector<Module*>::iterator locate_module(Module* m){
    for(auto i = modules.begin(); i != modules.end(); i++) if ((*i) == m) return i;
    return modules.end();
}

void remove_module(Module* m){
    if(!m) return;
    auto pos = locate_module(m);
    auto it = modules.erase(pos);
    for(; it != modules.end(); ++it) (*it)->xoffset -= m->panel_width + 1;
    last_x_offset -= m->panel_width + 1;
    mainwindow->cabinet.set_size_request((last_x_offset + 1)*gui_scale,-1);
    mainwindow->cabinet_scrolledwindow.set_size_request(-1,MODULE_PANEL_HEIGHT*gui_scale);
    delete m;
}
void remove_selected_module(){
    remove_module(selected_module);
    selected_module = NULL;
}

void swap_modules(std::vector<Module*>::iterator m1,std::vector<Module*>::iterator m2){
    Module* tmp_m = (*m1);
    int xoffset_low = (*m1)->xoffset;
    int xoffset_high = xoffset_low + (*m2)->panel_width + 1;
    (*m1) = (*m2);
    (*m2) = tmp_m;
    //Note: as the modules has been swapped, the iterators are now used in reverse order.
    (*m2)->xoffset = xoffset_high;
    (*m1)->xoffset = xoffset_low;
}

void move_selected_module_left(){
    if(!selected_module) return;
    auto pos = locate_module(selected_module);
    if(pos == modules.begin()) return;
    auto pos2 = pos-1;
    swap_modules(pos2,pos);
}
void move_selected_module_right(){
    if(!selected_module) return;
    auto pos = locate_module(selected_module);
    auto pos2 = pos+1;
    if(pos2 == modules.end()) return;
    swap_modules(pos,pos2);
}

// ===== DSP CALCULATIONS =====

/*
 * The key procedure that calculates all data.
 */
void do_dsp_cycle(AudioContext ctx){
    for(auto &w : wires)   w->pass_sample();
    for(auto &m : modules) m->dsp_full(ctx);
}

// ====== VIEW SETTINGS =======

double get_gui_scale() {return gui_scale;}
void set_gui_scale(double scale) {
    double rounder = 20.0;
    gui_scale = ((int)(scale*rounder + 0.5))/rounder;

    mainwindow->cabinet.set_size_request((last_x_offset + 1)*gui_scale,-1);
    mainwindow->cabinet_scrolledwindow.set_size_request(-1,MODULE_PANEL_HEIGHT*gui_scale);
    int width = mainwindow->get_width();
    mainwindow->resize(width,1);
    mainwindow->cabinet.queue_draw();
};
void zoom_in(){
    double newscale = gui_scale+0.05;
    if(newscale <= 1.0)
        set_gui_scale(newscale);
}
void zoom_out(){
    double newscale = gui_scale-0.05;
    if(newscale >= 0.3)
        set_gui_scale(newscale);
}

// ====== SELECTION and CONNECTIONS MANAGEMENT ======

void select_outlet(Outlet* outlet){
    unselect();
    selected_outlet = outlet;
    outlet->highlight = true;
}
void select_inlet(Inlet* inlet){
    if(!selected_outlet){
        unselect();
        selected_inlet = inlet;
        inlet->highlight = true;
    }else{
        //An outlet was selected before, thus we can create a connection!
        if(selected_outlet->connection != NULL && selected_outlet->connection == inlet->connection){
            //already connected
            disconnect(selected_outlet->connection);
            unselect();
        }else{

            //Disconnect existing connections to one of selected plugs
            if(inlet->connection){
                disconnect(inlet->connection);
            }

            if(selected_outlet->connection){
                disconnect(selected_outlet->connection);
            }

            //if( !(selected_outlet->parent == inlet->parent) ){ //make sure we're not connecting a module to itself
                connect(selected_outlet,inlet);
            //}
            unselect();
        }
    }
}
void select_module(Module* module){
    unselect();
    selected_module = module;
    module->highlight = true;
}
void unselect_inlet(){
    if(selected_inlet) selected_inlet->highlight = false;
    selected_inlet = NULL;
}
void unselect_outlet(){
    if(selected_outlet) selected_outlet->highlight = false;
    selected_outlet = NULL;
}
void unselect_module(){
    if(selected_module) selected_module->highlight = false;
    selected_module = NULL;
}
void unselect(){
    unselect_inlet();
    unselect_outlet();
    unselect_module();
    mainwindow->cabinet.queue_draw();
}

void connect(Outlet* from, Inlet* to){
    Wire* wire = new Wire(from,to);
    from->connection = wire;
    to->connection = wire;
    wires.insert(wire);
}

void disconnect(Wire* wire){
    if(!wire) return;
    wires.erase(wire);
    wire->from->connection = NULL;
    wire->to->connection = NULL;
    delete wire;
}

void register_inlet(Inlet* inlet){
    inlets.insert(inlet);
}
void register_outlet(Outlet* outlet){
    outlets.insert(outlet);
}
void register_knob(Knob* knob){
    knobs.insert(knob);
}
void register_switch(Switch* _switch){
    switches.insert(_switch);
}
void unregister_inlet(Inlet* inlet){
    inlets.erase(inlet);
}
void unregister_outlet(Outlet* outlet){
    outlets.erase(outlet);
}
void unregister_knob(Knob* knob){
    knobs.erase(knob);
}
void unregister_switch(Switch* _switch){
    switches.erase(_switch);
}

} //namespace Engine
