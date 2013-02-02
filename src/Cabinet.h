/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
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
