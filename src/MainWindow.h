/*
    Copyright (C) 2012, 2013 Rafał Cieślak

    Modified 2016 by Robert Gyllenberg 
     - added file saving and loading functionality

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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm-3.0/gtkmm.h>
#include "Cabinet.h"

class MainWindow : public Gtk::Window
{
    public:
        MainWindow();
        virtual ~MainWindow();

        Cabinet cabinet; //should be moved to private and refreshed with an exposed public method
        Gtk::ScrolledWindow cabinet_scrolledwindow;
    protected:
    private:
        Gtk::VBox main_vbox;
        Gtk::Toolbar toolbar;
        Gtk::HBox cabinet_add_box;

          //Tree model columns:
          class ModListCols : public Gtk::TreeModel::ColumnRecord
          {
          public:
            ModListCols()
            { add(id); add(fullname);}
            Gtk::TreeModelColumn<int> id;
            Gtk::TreeModelColumn<Glib::ustring> fullname;
          };

          ModListCols modlist_col;

        Glib::RefPtr<Gtk::TreeStore> modules_treemodel;
        Gtk::TreeView modules_list;


        Gtk::ToolButton toolbutton_add;
        Gtk::ToggleToolButton toolbutton_edit;
        Gtk::SeparatorToolItem toolbutton_sep;
        Gtk::ToolButton toolbutton_left;
        Gtk::ToolButton toolbutton_right;
        Gtk::ToolButton toolbutton_remove;
        Gtk::SeparatorToolItem toolbutton_sep2;
        Gtk::ToolButton toolbutton_zoomin;
        Gtk::ToolButton toolbutton_zoomout;
        Gtk::ToolButton toolbutton_save;
        Gtk::ToolButton toolbutton_load;



        bool on_delete_event(GdkEventAny* event);

        void on_add_clicked();
        void on_edit_clicked();
        void on_left_clicked();
        void on_right_clicked();
        void on_remove_clicked();
        void on_load_clicked();
        void on_save_clicked();
        void on_zoomin_clicked();
        void on_zoomout_clicked();
        bool adding;

        void on_modlist_item_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
};

#endif // MAINWINDOW_H
