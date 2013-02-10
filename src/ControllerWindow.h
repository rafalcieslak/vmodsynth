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

#ifndef CONTROLLERWINDOW_H
#define CONTROLLERWINDOW_H

#include <gtkmm-3.0/gtkmm.h>


class ControllerWindow : public Gtk::Dialog
{
    public:
        ControllerWindow(int ch = 0, int ctrl = 1);
        virtual ~ControllerWindow();
        Gtk::SpinButton channel;
        Gtk::SpinButton controller;
    protected:
    private:
        Gtk::Table table;
        Gtk::Label label1;
        Gtk::Label label2;
};

#endif // CONTROLLERWINDOW_H
