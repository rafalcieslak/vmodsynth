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


#include "ControllerWindow.h"
#include "MainWindow.h"

extern MainWindow* mainwindow;

ControllerWindow::ControllerWindow(int ch, int ctrl) : Gtk::Dialog("MIDI controller setup",*mainwindow,true),
                                                                  table(2,2),
                                                                  label1("Channel (0 for all):"),
                                                                  label2("Controller:")
{
    set_modal(true);
    set_resizable(false);
    add_button("Cancel",Gtk::ResponseType::RESPONSE_CANCEL);
    add_button("Unbind",Gtk::ResponseType::RESPONSE_NO);
    add_button("Bind",Gtk::ResponseType::RESPONSE_YES);
    set_default_response(Gtk::ResponseType::RESPONSE_YES);
    channel.set_range(0.0,16.0);
    controller.set_range(1.0,127.0);
    channel.set_digits(0);
    controller.set_digits(0);
    channel.set_increments(1.0,16.0);
    controller.set_increments(1.0,16.0);
    channel.set_value(ch);
    controller.set_value(ctrl);
    channel.set_activates_default(true);
    controller.set_activates_default(true);
    table.attach(label1    ,0,1,0,1,Gtk::AttachOptions::EXPAND,Gtk::AttachOptions::SHRINK);
    table.attach(label2    ,0,1,1,2,Gtk::AttachOptions::EXPAND,Gtk::AttachOptions::SHRINK);
    table.attach(channel   ,1,2,0,1,Gtk::AttachOptions::SHRINK,Gtk::AttachOptions::SHRINK);
    table.attach(controller,1,2,1,2,Gtk::AttachOptions::SHRINK,Gtk::AttachOptions::SHRINK);
    Gtk::Box* vbox = get_vbox();
    vbox->pack_start(table);
    vbox->show_all();
}

ControllerWindow::~ControllerWindow()
{
    //dtor
}
