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

#ifndef CABINET_H
#define CABINET_H

#include <gtkmm-3.0/gtkmm.h>
#include <cairomm/cairomm.h>

class Cabinet : public Gtk::DrawingArea
{
    public:
        Cabinet();
        virtual ~Cabinet();
        void set_edit_mode(bool e);
    protected:
    private:
        bool edit_mode;

        bool on_draw (const Cairo::RefPtr<Cairo::Context>& cr);
        //bool on_button_pressed(GdkEventButton * event);
        bool on_motion_notify_event(GdkEventMotion * event);
        bool on_button_press_event(GdkEventButton * event);
        bool on_button_release_event(GdkEventButton * event);
        bool on_scroll_event(GdkEventScroll * event);
};

#endif // CABINET_H
