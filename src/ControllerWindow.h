/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
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
