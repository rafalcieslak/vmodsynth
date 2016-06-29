/*
    Copyright (C) 2012, 2013 Rafał Cieślak

    Modified 2016 by Robert Gyllenberg 
     - implemented file saving and loading procedures.

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


#ifndef ENGINE_H
#define ENGINE_H

#include <set>
#include <vector>
#include "Module.h"
#include "Inlet.h"
#include "Outlet.h"
#include "Switch.h"
#include "Wire.h"

namespace Engine{

#ifdef ENGINE_DEFS
std::vector<Module* > modules;
std::set<Inlet* > inlets;
std::set<Outlet* > outlets;
std::vector<Knob* > knobs;
std::vector<Switch* > switches;
std::set<Wire* > wires;
#else
extern std::vector<Module* > modules;
extern std::set<Inlet* > inlets;
extern std::set<Outlet* > outlets; 	
extern std::vector<Knob* > knobs;
extern std::vector<Switch* > switches;extern std::set<Wire* > wires;

extern Module* selected_module;
#endif

void clear_rack(); //Removes all modules
void dump_patch(std::string);
void parse_file(std::string);
void save_patch();
void load_patch();

void create_and_append_module(int ID);
void remove_module(Module* m);
void remove_selected_module();

void move_selected_module_left();
void move_selected_module_right();

void do_dsp_cycle(AudioContext ctx);

double get_gui_scale();
void zoom_in();
void zoom_out();

void select_inlet(Inlet* inlet);
void select_outlet(Outlet* outlet);
void select_module(Module* module);
void unselect_inlet();
void unselect_outlet();
void unselect_module();
void unselect();

void connect(Outlet* from, Inlet* to);
void disconnect(Wire* wire);

void register_inlet(Inlet* inlet);
void register_outlet(Outlet* outlet);
void register_knob(Knob* knob);
void register_switch(Switch* _switch);
void unregister_inlet(Inlet* inlet);
void unregister_outlet(Outlet* outlet);
void unregister_knob(Knob* knob);
void unregister_switch(Switch* _switch);

} //namespace Engine
#endif // ENGINE_H
