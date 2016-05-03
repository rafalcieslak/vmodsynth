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


#include <gtkmm-3.0/gtkmm.h>
#include "MainWindow.h"
#include "Engine.h"

MainWindow::MainWindow() :

                            toolbutton_add(Gtk::Stock::ADD),
                            toolbutton_edit(Gtk::Stock::EDIT),
                            toolbutton_left(Gtk::Stock::GO_BACK),
                            toolbutton_right(Gtk::Stock::GO_FORWARD),
                            toolbutton_remove(Gtk::Stock::REMOVE),
                            toolbutton_zoomin(Gtk::Stock::ZOOM_IN),
                            toolbutton_zoomout(Gtk::Stock::ZOOM_OUT),
                            toolbutton_save(Gtk::Stock::SAVE), 
                            toolbutton_load(Gtk::Stock::OPEN),
                            adding(false)
{
    set_title("vModSynth");
    set_default_size(900,300);
    //set_resizable(false);

    main_vbox.pack_start(toolbar,false,false);
    main_vbox.pack_start(cabinet_add_box,true,true);

    toolbar.append(toolbutton_edit);
    toolbar.append(toolbutton_sep);
    toolbar.append(toolbutton_add);
    toolbar.append(toolbutton_left);
    toolbar.append(toolbutton_right);
    toolbar.append(toolbutton_remove);
    toolbar.append(toolbutton_sep2);
    toolbar.append(toolbutton_load); 
    toolbar.append(toolbutton_save); 
    toolbar.append(toolbutton_zoomin);
    toolbar.append(toolbutton_zoomout);

    Glib::RefPtr<Gtk::AccelGroup> accel_group = Gtk::AccelGroup::create();
    add_accel_group(accel_group);

    toolbutton_add.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_add_clicked));
    toolbutton_edit.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_edit_clicked));
    toolbutton_left.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_left_clicked));
    toolbutton_right.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_right_clicked));
    toolbutton_remove.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_remove_clicked));
    toolbutton_load.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_load_clicked));
    toolbutton_save.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_save_clicked)); 
    toolbutton_zoomin.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_zoomin_clicked));
    toolbutton_zoomout.signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::on_zoomout_clicked));


    toolbutton_add.add_accelerator("clicked", accel_group, GDK_KEY_plus, (Gdk::ModifierType) 0, (Gtk::AccelFlags) 0);
    toolbutton_left.add_accelerator("clicked", accel_group, GDK_KEY_Left, Gdk::CONTROL_MASK, (Gtk::AccelFlags) 0);
    toolbutton_right.add_accelerator("clicked", accel_group, GDK_KEY_Right, Gdk::CONTROL_MASK, (Gtk::AccelFlags) 0);
    toolbutton_remove.add_accelerator("clicked", accel_group, GDK_KEY_Delete, (Gdk::ModifierType) 0, (Gtk::AccelFlags) 0);
    toolbutton_zoomin.add_accelerator("clicked", accel_group, GDK_KEY_plus, Gdk::CONTROL_MASK, (Gtk::AccelFlags) 0);
    toolbutton_zoomout.add_accelerator("clicked", accel_group, GDK_KEY_minus, Gdk::CONTROL_MASK, (Gtk::AccelFlags) 0);

    //black toolbars in ubuntu
    Glib::RefPtr<Gtk::StyleContext> sc = toolbar.get_style_context();
    sc->add_class("primary-toolbar");

    cabinet_add_box.pack_start(modules_list,false,false);
    cabinet_add_box.pack_start(cabinet_scrolledwindow,true,true);
    cabinet_scrolledwindow.add(cabinet);
    cabinet_scrolledwindow.set_policy(Gtk::POLICY_ALWAYS,Gtk::POLICY_NEVER);
    cabinet_scrolledwindow.set_size_request(-1,MODULE_PANEL_HEIGHT*Engine::get_gui_scale());

    modules_treemodel = Gtk::TreeStore::create(modlist_col);
    modules_list.set_model(modules_treemodel);

    Gtk::TreeModel::Row row_cat_sources = *(modules_treemodel->append());
    row_cat_sources[modlist_col.id] = 0;
    row_cat_sources[modlist_col.fullname] = "Signal sources";
    Gtk::TreeModel::Row row_cat_signal = *(modules_treemodel->append());
    row_cat_signal[modlist_col.id] = 0;
    row_cat_signal[modlist_col.fullname] = "Signal processing";
    Gtk::TreeModel::Row row_cat_effects = *(modules_treemodel->append());
    row_cat_effects[modlist_col.id] = 0;
    row_cat_effects[modlist_col.fullname] = "Effect modules";
    Gtk::TreeModel::Row row_cat_system = *(modules_treemodel->append());
    row_cat_system[modlist_col.id] = 0;
    row_cat_system[modlist_col.fullname] = "System modules";

    Gtk::TreeModel::Row row;

    //System modules
    row = *(modules_treemodel->append(row_cat_system.children()));
    row[modlist_col.id] = 1001;
    row[modlist_col.fullname] = "Audio output";
    row = *(modules_treemodel->append(row_cat_system.children()));
    row[modlist_col.id] = 1005;
    row[modlist_col.fullname] = "MIDI input";

    //Signal sources
    row = *(modules_treemodel->append(row_cat_sources.children()));
    row[modlist_col.id] = 100;
    row[modlist_col.fullname] = "Oscillator";

    row = *(modules_treemodel->append(row_cat_sources.children()));
    row[modlist_col.id] = 101;
    row[modlist_col.fullname] = "Noise source";

    row = *(modules_treemodel->append(row_cat_sources.children()));
    row[modlist_col.id] = 213;
    row[modlist_col.fullname] = "Random Note";

    //Processing
    row = *(modules_treemodel->append(row_cat_signal.children()));
    row[modlist_col.id] = 210;
    row[modlist_col.fullname] = "Amplifier";

    row = *(modules_treemodel->append(row_cat_signal.children()));
    row[modlist_col.id] = 230;
    row[modlist_col.fullname] = "ADSR envelope";

    row = *(modules_treemodel->append(row_cat_signal.children()));
    row[modlist_col.id] = 300;
    row[modlist_col.fullname] = "Filter";

    row = *(modules_treemodel->append(row_cat_signal.children()));
    row[modlist_col.id] = 400;
    row[modlist_col.fullname] = "2ch mixer";

    row = *(modules_treemodel->append(row_cat_signal.children()));
    row[modlist_col.id] = 200;
    row[modlist_col.fullname] = "Multiply";

    row = *(modules_treemodel->append(row_cat_signal.children()));
    row[modlist_col.id] = 201;
    row[modlist_col.fullname] = "Panorama/Crossfade";

    row = *(modules_treemodel->append(row_cat_signal.children()));
    row[modlist_col.id] = 203;
    row[modlist_col.fullname] = "Sample and hold";

    row = *(modules_treemodel->append(row_cat_effects.children()));
    row[modlist_col.id] = 211;
    row[modlist_col.fullname] = "Sum";

    row = *(modules_treemodel->append(row_cat_signal.children()));
    row[modlist_col.id] = 212;
    row[modlist_col.fullname] = "Gain + Offset";

    row = *(modules_treemodel->append(row_cat_signal.children()));
    row[modlist_col.id] = 701;
    row[modlist_col.fullname] = "Echo";

    row = *(modules_treemodel->append(row_cat_effects.children()));
    row[modlist_col.id] = 703;
    row[modlist_col.fullname] = "Reverb";

    row = *(modules_treemodel->append(row_cat_effects.children()));
    row[modlist_col.id] = 702;
    row[modlist_col.fullname] = "Overdrive";

    //modules_list.append_column("", modlist_col.id);
    modules_list.append_column("Modules", modlist_col.fullname);
    modules_list.set_size_request(136,-1);
    modules_list.expand_all();

    modules_list.signal_row_activated().connect(sigc::mem_fun(*this,&MainWindow::on_modlist_item_activated));

    add(main_vbox);

    show_all();
    modules_list.hide();

    toolbutton_edit.set_active(1);
}

MainWindow::~MainWindow()
{

}

bool MainWindow::on_delete_event(GdkEventAny*){
    Gtk::Main::quit();
    return true;
}

void MainWindow::on_add_clicked(){
    if(!adding){
        modules_list.show();
        adding = true;
    }else{
        modules_list.hide();
        adding = false;
    }
}

void MainWindow::on_edit_clicked(){
    bool e = toolbutton_edit.get_active();
    if(e){
        //entering edit mode
        cabinet.set_edit_mode(true);
        toolbutton_remove.set_sensitive(true);
        toolbutton_left.set_sensitive(true);
        toolbutton_right.set_sensitive(true);
        toolbutton_add.set_sensitive(true);
    }else{
        //leaving edit mode
        cabinet.set_edit_mode(false);
        modules_list.hide();
        adding = false;
        toolbutton_remove.set_sensitive(false);
        toolbutton_left.set_sensitive(false);
        toolbutton_right.set_sensitive(false);
        toolbutton_add.set_sensitive(false);
    }
}

void MainWindow::on_left_clicked(){
    Engine::move_selected_module_left();
    cabinet.queue_draw();
}

void MainWindow::on_right_clicked(){
    Engine::move_selected_module_right();
    cabinet.queue_draw();
}

void MainWindow::on_remove_clicked(){
    Engine::remove_selected_module();
    cabinet.queue_draw();
}

void MainWindow::on_zoomin_clicked(){
    Engine::zoom_in();
}

void MainWindow::on_zoomout_clicked(){
    Engine::zoom_out();
}

 void MainWindow::on_save_clicked(){ //RG2016
     //Engine::dump_patch();
     Engine::save_patch();
 }
 
 void MainWindow::on_load_clicked(){ //RG2016
     Engine::load_patch();
 }


void MainWindow::on_modlist_item_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn*){

    Gtk::TreeModel::iterator iter = modules_treemodel->get_iter(path);
    if(iter)
    {
        Gtk::TreeModel::Row row = *iter;
        int id = row[modlist_col.id];
        if(id != 0){
            Engine::create_and_append_module(id);
            cabinet.queue_draw();
        }else{
            Gtk::TreePath path(row);
            if(modules_list.row_expanded(path)){
                modules_list.collapse_row(path);
            }else{
                modules_list.expand_row(path,false);
            }

        }
    }
}
