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


#include "Cabinet.h"
#include "Engine.h"
#include "Knob.h"
#include "MainWindow.h"
#include "ControllerWindow.h"

extern MainWindow* mainwindow;

Knob *dragged_knob = NULL;
bool drag_treshold_passed;
double drag_start_x = 0.0;
double drag_start_y = 0.0;

Cabinet::Cabinet()
{
    //set_size_request(-1,MODULE_PANEL_HEIGHT);
    add_events(Gdk::EventMask::BUTTON_PRESS_MASK);
    add_events(Gdk::EventMask::BUTTON1_MOTION_MASK);
    add_events(Gdk::EventMask::BUTTON_RELEASE_MASK);
    add_events(Gdk::EventMask::SCROLL_MASK);
    //signal_button_press_event().connect(sigc::mem_fun(*this,&Cabinet::on_button_pressed));
}

Cabinet::~Cabinet()
{
    //dtor
}

void Cabinet::set_edit_mode(bool e){
    edit_mode = e;
    if(!edit_mode){
        //leaving edit mode
        Engine::unselect();
    }
}

bool Cabinet::on_draw (const Cairo::RefPtr<Cairo::Context>& cr){
    cr->save();
    double scale = Engine::get_gui_scale();
    cr->scale(scale,scale);
    for(auto &i : Engine::modules){
        i->draw(cr);
    }
    for(auto &i : Engine::inlets){
        i->draw(cr);
    }
    for(auto &i : Engine::outlets){
        i->draw(cr);
    }
    for(auto &i : Engine::knobs){
        i->draw(cr);
    }
    for(auto &i : Engine::switches){
        i->draw(cr);
    }
    for(auto &i : Engine::wires){
        i->draw(cr);
    }
    cr->restore();
    return false;
}

bool Cabinet::on_motion_notify_event(GdkEventMotion * event){
    if(dragged_knob){
        double scale = Engine::get_gui_scale();
        const double x = event->x/scale - drag_start_x;
        const double y = event->y/scale - drag_start_y;
        if(drag_treshold_passed){
            //std::cout << "Movement to " << x << " " << y << ".\n";
            dragged_knob->movement_position(x,y);
        }else{
            const int treshold = 10;
            if(x > treshold || x < -treshold || y > treshold || y < -treshold)
                drag_treshold_passed = true;
        }
    }
    return false;
}

bool Cabinet::on_button_press_event(GdkEventButton * event){
    double scale = Engine::get_gui_scale();
    double x = event->x / scale;
    double y = event->y / scale;
    //if(Engine::selected_module)
        //std::cout << "Mouse down at " << x - Engine::selected_module->xoffset << " " << y << ".\n";
    if(event->type == GDK_BUTTON_PRESS){
        if(event->button == 1){

            if(edit_mode){
                //search for an inlet
                Inlet * inlet = NULL;
                for(auto &i : Engine::inlets)
                    if(i->is_point_within(x,y)){
                        inlet = i;
                        break;
                    }

                if(inlet!=NULL){
                    Engine::select_inlet(inlet);
                }else{
                    //search for an outlet
                    Outlet* outlet = NULL;
                    for(auto &o : Engine::outlets)
                        if(o->is_point_within(x,y)){
                            outlet = o;
                            break;
                        }

                    if(outlet!=NULL){
                        Engine::select_outlet(outlet);
                    }else{
                        //search for a module
                        Module* module = NULL;
                        for(auto &m : Engine::modules)
                            if(m->is_point_within(x,y)){
                                module = m;
                                break;
                            }
                        if(module!=NULL){
                            Engine::select_module(module);
                        }else{
                            Engine::unselect();
                        }
                    }

                }
            } // end if editmode

            //search for a knob
            Knob* knob = NULL;
            for(auto &m : Engine::knobs)
                if(m->is_point_within(x,y)){
                    knob = m;
                    break;
                }

            if(knob!=NULL){
                //Starting to drag a knob!
                dragged_knob = knob;
                drag_treshold_passed = false;
                drag_start_x = x;
                drag_start_y = y;
                dragged_knob->movement_start();
            }else{
                //search for a switch
                Switch* _switch = NULL;
                for(auto &m : Engine::switches)
                    if(m->is_point_within(x,y)){
                        _switch = m;
                        break;
                    }

                if(_switch!=NULL){
                    _switch->set_value(!_switch->get_value());
                }
            }


        }else if(event->button == 3){ //LMB
            //search for a knob
            Knob* knob = NULL;
            for(auto &m : Engine::knobs)
                if(m->is_point_within(x,y)){
                    knob = m;
                    break;
                }

            if(knob!=NULL){
                //A knob was left-clicked. Suggest setting up a controller...
                int ch = 0, ctrl = 1;
                if(knob->controlled){
                    ch   = knob->channel;
                    ctrl = knob->controller;
                }
                ControllerWindow ctrlw(ch,ctrl);
                int r = ctrlw.run();
                if( r == Gtk::ResponseType::RESPONSE_YES){ // bind the controller link
                    knob->controlled = true;
                    knob->controller = ctrlw.controller.get_value();
                    knob->channel    = ctrlw.channel.get_value();
                    knob->redraw_me();
                }else if( r == Gtk::ResponseType::RESPONSE_NO){ // unbind the controller
                    knob->controlled = false;
                    knob->redraw_me();
                } //other replies mean cancelation
            }
        }
    } // if event type == button press
    return false;
}

bool Cabinet::on_button_release_event(GdkEventButton * event){
    if(dragged_knob){
        dragged_knob->movement_end();
        dragged_knob = NULL;
    }
    return false;
}

bool Cabinet::on_scroll_event(GdkEventScroll * event){
    double scale = Engine::get_gui_scale();
    double x = event->x / scale;
    double y = event->y / scale;

    if (event->state & Gdk::ModifierType::CONTROL_MASK){
        //controll key is held, thus react by zooming
        if(event->direction){
            //up
            Engine::zoom_in();
        }else{
            //down
            Engine::zoom_out();
        }
    }else{
        //no modifiers, process as normal scroll
            auto adj = mainwindow->cabinet_scrolledwindow.get_hadjustment();
            if(event->direction == 0){
                //scrolled up
                adj->set_value(adj->get_value() - adj->get_step_increment());
            }else{
                //scrolled down
                adj->set_value(adj->get_value() + adj->get_step_increment());
            }
    }

    return true; //DO NOT propagate the signal handle further!
}
