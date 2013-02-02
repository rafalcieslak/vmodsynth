/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#ifndef ENGINE_H
#define ENGINE_h

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
std::set<Knob* > knobs;
std::set<Switch* > switches;
std::set<Wire* > wires;
#else
extern std::vector<Module* > modules;
extern std::set<Inlet* > inlets;
extern std::set<Outlet* > outlets;
extern std::set<Knob* > knobs;
extern std::set<Switch* > switches;
extern std::set<Wire* > wires;

extern Module* selected_module;
#endif

void create_and_append_module(int ID);
void remove_module(Module* m);
void remove_selected_module();

void move_selected_module_left();
void move_selected_module_right();

void do_dsp_cycle();

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
