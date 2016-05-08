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

#include <iostream>
#include <set>
#include <fstream>
#include "Selector.h"
#include <libxml++/libxml++.h>
#include <libxml/tree.h>

#include <cstdlib>

#define ENGINE_DEFS
#include "Engine.h"
#undef ENGINE_DEFS

#include "Module.h"
#include "ModuleList.h"
#include "Wire.h"

#include "MainWindow.h"
extern MainWindow *mainwindow;

namespace Engine{

Inlet* selected_inlet = NULL;
Outlet* selected_outlet = NULL;
Module* selected_module = NULL;

int last_x_offset = 0;

double gui_scale = 0.8;

// ====== ADDING, REMOVING and MOVING modules ============

void create_and_append_module(int ID){
    Module* newmod = create_new_module_from_ID(ID);
    if (newmod == NULL) //failed to create
        return;
    modules.push_back(newmod);
    newmod->xoffset = last_x_offset;
    last_x_offset += newmod->panel_width + 1;
    mainwindow->cabinet.set_size_request((last_x_offset + 1)*gui_scale,-1);
    mainwindow->cabinet_scrolledwindow.set_size_request(-1,MODULE_PANEL_HEIGHT*gui_scale);
}

std::vector<Module*>::iterator locate_module(Module* m){
    for(auto i = modules.begin(); i != modules.end(); i++) if ((*i) == m) return i;
    return modules.end();
}

void remove_module(Module* m){
    if(!m) return;
    auto pos = locate_module(m);
    auto it = modules.erase(pos);
    for(; it != modules.end(); ++it) (*it)->xoffset -= m->panel_width + 1;
    last_x_offset -= m->panel_width + 1;
    mainwindow->cabinet.set_size_request((last_x_offset + 1)*gui_scale,-1);
    mainwindow->cabinet_scrolledwindow.set_size_request(-1,MODULE_PANEL_HEIGHT*gui_scale);
    delete m;
}
void remove_selected_module(){
    remove_module(selected_module);
    selected_module = NULL;
}

void swap_modules(std::vector<Module*>::iterator m1,std::vector<Module*>::iterator m2){
    Module* tmp_m = (*m1);
    int xoffset_low = (*m1)->xoffset;
    int xoffset_high = xoffset_low + (*m2)->panel_width + 1;
    (*m1) = (*m2);
    (*m2) = tmp_m;
    //Note: as the modules has been swapped, the iterators are now used in reverse order.
    (*m2)->xoffset = xoffset_high;
    (*m1)->xoffset = xoffset_low;
}

void move_selected_module_left(){
    if(!selected_module) return;
    auto pos = locate_module(selected_module);
    if(pos == modules.begin()) return;
    auto pos2 = pos-1;
    swap_modules(pos2,pos);
}
void move_selected_module_right(){
    if(!selected_module) return;
    auto pos = locate_module(selected_module);
    auto pos2 = pos+1;
    if(pos2 == modules.end()) return;
    swap_modules(pos,pos2);
}

// ===== DSP CALCULATIONS =====

/*
 * The key procedure that calculates all data.
 */
void do_dsp_cycle(){
    for(auto &w : wires)   w->pass_sample();
    for(auto &m : modules) m->dsp();
}

// ====== VIEW SETTINGS =======

double get_gui_scale() {return gui_scale;}
void set_gui_scale(double scale) {
    double rounder = 20.0;
    gui_scale = ((int)(scale*rounder + 0.5))/rounder;

    mainwindow->cabinet.set_size_request((last_x_offset + 1)*gui_scale,-1);
    mainwindow->cabinet_scrolledwindow.set_size_request(-1,MODULE_PANEL_HEIGHT*gui_scale);
    int width = mainwindow->get_width();
    mainwindow->resize(width,1);
    mainwindow->cabinet.queue_draw();
};
void zoom_in(){
    double newscale = gui_scale+0.05;
    if(newscale <= 1.0)
        set_gui_scale(newscale);
}
void zoom_out(){
    double newscale = gui_scale-0.05;
    if(newscale >= 0.3)
        set_gui_scale(newscale);
}

// ====== SELECTION and CONNECTIONS MANAGEMENT ======

void select_outlet(Outlet* outlet){
    unselect();
    selected_outlet = outlet;
    outlet->highlight = true;
}
void select_inlet(Inlet* inlet){
    if(!selected_outlet){
        unselect();
        selected_inlet = inlet;
        inlet->highlight = true;
    }else{
        //An outlet was selected before, thus we can create a connection!
        if(selected_outlet->connection != NULL && selected_outlet->connection == inlet->connection){
            //already connected
            disconnect(selected_outlet->connection);
            unselect();
        }else{

            //Disconnect existing connections to one of selected plugs
            if(inlet->connection){
                disconnect(inlet->connection);
            }

            if(selected_outlet->connection){
                disconnect(selected_outlet->connection);
            }

            //if( !(selected_outlet->parent == inlet->parent) ){ //make sure we're not connecting a module to itself
                connect(selected_outlet,inlet);
            //}
            unselect();
        }
    }
}
void select_module(Module* module){
    unselect();
    selected_module = module;
    module->highlight = true;
}
void unselect_inlet(){
    if(selected_inlet) selected_inlet->highlight = false;
    selected_inlet = NULL;
}
void unselect_outlet(){
    if(selected_outlet) selected_outlet->highlight = false;
    selected_outlet = NULL;
}
void unselect_module(){
    if(selected_module) selected_module->highlight = false;
    selected_module = NULL;
}
void unselect(){
    unselect_inlet();
    unselect_outlet();
    unselect_module();
    mainwindow->cabinet.queue_draw();
}

void connect(Outlet* from, Inlet* to){
    Wire* wire = new Wire(from,to);
    from->connection = wire;
    to->connection = wire;
    wires.insert(wire);
}

void disconnect(Wire* wire){
    if(!wire) return;
    wires.erase(wire);
    wire->from->connection = NULL;
    wire->to->connection = NULL;
    delete wire;
}

void register_inlet(Inlet* inlet){
    inlets.insert(inlet);
}
void register_outlet(Outlet* outlet){
    outlets.insert(outlet);
}
void register_knob(Knob* knob){
    knobs.push_back(knob);
}
void register_switch(Switch* _switch){
    switches.push_back(_switch);
}
void unregister_inlet(Inlet* inlet){
    inlets.erase(inlet);
}
void unregister_outlet(Outlet* outlet){
    outlets.erase(outlet);
}

void unregister_knob(Knob* knob){
    for(auto i = knobs.begin(); i != knobs.end(); i++)
    {
        if ((*i) ==knob)
        { 
          knobs.erase(i);
          break;
        }
    }
}

void unregister_switch(Switch* _switch){
    for(auto i = switches.begin(); i != switches.end(); i++)
    {
        if ((*i) == _switch)
        { 
          switches.erase(i);
          break;
        }
    }

}

int get_mod_pos(Module* m){
     int n=0;
     for(auto i = modules.begin(); i != modules.end(); i++)
     {
        if ((*i) == m) return n;
        n++;
     }
     return n;
}

// ================== SAVING AND LOADING ===================

void dump_patch(std::string filename) 
//Dumps XML-data to a file
{
    xmlDocPtr doc = NULL;       /* document pointer */
    xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;/* node pointers */
    xmlDtdPtr dtd = NULL;       /* DTD pointer */

    xmlNodePtr some_section     = NULL;
    xmlNodePtr some_entry       = NULL;
    xmlNodePtr module_entry     = NULL;
    xmlNodePtr module_section   = NULL;
    xmlIndentTreeOutput = 1;
  
    char buff[256];

    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "patch");
    xmlDocSetRootElement(doc, root_node);
    xmlNewProp(root_node, BAD_CAST "vmodsynth_version", BAD_CAST "1.1");

    char origLocale[1024];
    strcpy(origLocale, std::setlocale(LC_ALL, "")); //Store locale before changing
    std::setlocale(LC_ALL, "C"); // We use '.' as decimal-point separator

    some_section = xmlNewChild(root_node, NULL, BAD_CAST "modules", BAD_CAST "");

    int m_pos=0, n=0;
    //Iterate over modules
    for(auto m = modules.begin(); m != modules.end(); m++)
    { 
        module_entry = xmlNewChild(some_section, NULL, BAD_CAST "module",BAD_CAST "");
        xmlNewProp(module_entry, BAD_CAST "name", BAD_CAST((*m)->name.c_str()));
        //Module name is not used, but output for human readability of the file. 
        xmlNewProp(module_entry, BAD_CAST "type_id", BAD_CAST(std::to_string((*m)->type_id).c_str()));

        n=0;
        //knob-settings per module
        for(auto i = (*m)->knobs.begin(); i != (*m)->knobs.end(); i++)
        { 
            double value = (*i)->get_value();
            some_entry = xmlNewChild(module_entry, NULL, BAD_CAST "knob",BAD_CAST "");
            xmlNewProp(some_entry, BAD_CAST "n", BAD_CAST(std::to_string(n).c_str()));
            xmlNewProp(some_entry, BAD_CAST "value", BAD_CAST(std::to_string(value).c_str()));
            n++; //Increment knob counter
        } //End knobs
 
        n=0;
        //swtich-settings per module
        for(auto i = (*m)->switches.begin(); i != (*m)->switches.end(); i++)
        { 
            double value = (*i)->get_value();
            some_entry = xmlNewChild(module_entry, NULL, BAD_CAST "switch",BAD_CAST "");
            xmlNewProp(some_entry, BAD_CAST "n", BAD_CAST(std::to_string(n).c_str()));
            xmlNewProp(some_entry, BAD_CAST "value", BAD_CAST(std::to_string(value).c_str()));
            n++; //Increment knob counter
        }
 

   } //End modules
 
    some_section = xmlNewChild(root_node, NULL, BAD_CAST "wires", BAD_CAST "");

    for(auto i = wires.begin(); i != wires.end(); i++)
    { 
        some_entry = xmlNewChild(some_section, NULL, BAD_CAST "wire",BAD_CAST "");

        n = get_mod_pos((*i)->from->parent);
        int o_n = (*i)->from->parent->get_outlet_index((*i)->from); //Logical order of outlet within module (eg. 0,1,2,...)

        xmlNewProp(some_entry, BAD_CAST "src", BAD_CAST(std::to_string(n).c_str()));
        xmlNewProp(some_entry, BAD_CAST "sj", BAD_CAST(std::to_string(o_n).c_str()));

        n = get_mod_pos((*i)->to->parent);
        int i_n = (*i)->to->parent->get_inlet_index((*i)->to); //Logical order of inlet within module (eg. 0,1,2,...)
        xmlNewProp(some_entry, BAD_CAST "dst", BAD_CAST(std::to_string(n).c_str()));
        xmlNewProp(some_entry, BAD_CAST "dj", BAD_CAST(std::to_string(i_n).c_str()));
    }

    xmlSaveFormatFileEnc(filename.c_str(), doc, "UTF-8", 1);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    std::setlocale(LC_ALL, origLocale); //Restore locale to the original state
}

void save_patch() //Actually save XML-data to an external file
{
    GtkWidget *dialog;
    std::string filename;
    dialog = gtk_file_chooser_dialog_new(
        "Save File",
        (GtkWindow*)(mainwindow),
        GTK_FILE_CHOOSER_ACTION_SAVE,
        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
        GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
        NULL);
    gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
    gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), ""); //Default directory
    gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), "untitled.xml");
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        dump_patch(filename);
        g_free (filename);
    }
    gtk_widget_destroy (dialog);
}//end save_patch()

void parcefile(std::string filepath)
{
 xmlpp::DomParser parser;
 parser.parse_file(filepath);
 const xmlpp::Node* rootNode = parser.get_document()->get_root_node();
 xmlpp::Node::NodeList mainList = rootNode->get_children();
 char origLocale[1024];
 strcpy(origLocale, std::setlocale(LC_ALL, "")); //Store locale before changing
 std::setlocale(LC_ALL, "C"); // We use '.' as decimal-point separator

 int m_pos=-1; //For keeping track on the current module position
 for(xmlpp::Node::NodeList::iterator iter = mainList.begin(); iter != mainList.end(); ++iter)
 {
     std::string contName= (*iter)->get_name();
     if(contName=="modules")
     {
         xmlpp::Element::NodeList modulesList = (*iter)->get_children();
         for(xmlpp::Element::NodeList::iterator modIter = modulesList.begin(); modIter != modulesList.end(); ++modIter)
         {
             std::string nodeType=(*modIter)->get_name();
             if(nodeType=="module")
             {             
                  m_pos++; //First occurence will be 0, second 1 etc...
                  const xmlpp::Element* element1 = dynamic_cast<const xmlpp::Element*>(*modIter);
                  std::string moduleName = element1->get_attribute_value("name"); //Not actually used here
                  std::string moduleId   = element1->get_attribute_value("type_id");
                  int module_type_id = std::stoi(moduleId); 

                  create_and_append_module(module_type_id); //Append module to rack
                  Module* current_module = modules[m_pos];

                  //Check for aggregated knob, switch etc. data within module entry:
                  xmlpp::Element::NodeList moduleDataList = (*modIter)->get_children();
                  for(xmlpp::Element::NodeList::iterator modDataIter = moduleDataList.begin(); modDataIter != moduleDataList.end(); ++modDataIter)
                  {
                      std::string nodeDataType=(*modDataIter)->get_name();
                      if(nodeDataType=="knob")
                      {
                          const xmlpp::Element* element2m = dynamic_cast<const xmlpp::Element*>(*modDataIter);
                          int    knob_n = std::stoi(element2m->get_attribute_value("n"));
                          double value  = std::stof(element2m->get_attribute_value("value"));
                          //std::cerr << "Module:" << m_pos << "Knob#" << knob_n << "value:" << value <<"\n"; 
                          
                          if((dynamic_cast<Selector*>(current_module->knobs[knob_n])))
                          {
                              //This Knob is, in fact a Selector object, treat differently 
                              Selector* someSelector = (Selector*)current_module->knobs[knob_n]; 
                              someSelector->set_value(value);
                              //std::cerr << "Is selector\n";
                          }
                          else
                          {
                               //Treat as basic Knob object
                               current_module->knobs[knob_n]->set_value(value); 
                               //std::cerr << "Is knob\n";
                          }
                      }
                      if(nodeDataType=="switch")
                      {
                          const xmlpp::Element* element3m = dynamic_cast<const xmlpp::Element*>(*modDataIter);
                          int    switch_n = std::stoi(element3m->get_attribute_value("n"));
                          double value = std::stof(element3m->get_attribute_value("value"));
                          Switch* theSwitch = current_module->switches[switch_n];
                          theSwitch->set_value(value);
                      }
                  }
             }
         }
     }

     if(contName=="wires")
     {
         xmlpp::Element::NodeList wiresList = (*iter)->get_children();
         for(xmlpp::Element::NodeList::iterator wireIter = wiresList.begin(); wireIter != wiresList.end(); ++wireIter)
         {
             std::string nodeType=(*wireIter)->get_name();
             if(nodeType=="wire")
             {
                 const xmlpp::Element* element = dynamic_cast<const xmlpp::Element*>(*wireIter);
                 int src = std::stoi(element->get_attribute_value("src"));
                 int sj  = std::stoi(element->get_attribute_value("sj"));
                 int dst = std::stoi(element->get_attribute_value("dst"));
                 int dj  = std::stoi(element->get_attribute_value("dj"));
                 Module* srcMod = modules[src];
                 Module* dstMod = modules[dst];
                 connect(srcMod->outlets[sj], dstMod->inlets[dj]);
             }
         }
     }

     if(contName=="knobs")
     {
         xmlpp::Element::NodeList knobList = (*iter)->get_children();
         int i=0;
         for(xmlpp::Element::NodeList::iterator knobIter = knobList.begin(); knobIter != knobList.end(); ++knobIter)
         {
             std::string nodeType=(*knobIter)->get_name();
             if(nodeType=="knob")
             {
                 const xmlpp::Element* element2 = dynamic_cast<const xmlpp::Element*>(*knobIter);
                 double value = std::stof(element2->get_attribute_value("value"));
                 if(typeid(*knobs[i]) == typeid(Selector))
                 {
                     //This Knob is, in fact a Selector object, treat differently 
                     Selector* someSelector = (Selector*)knobs[i]; 
                     someSelector->set_value(value);
                 }
                 else
                 {
                     //Treat as basic Knob object
                     knobs[i]->set_value(value); 
                 }
                 i++;
             }
         }
     }

     if(contName=="switches")
     {
         xmlpp::Element::NodeList switchList = (*iter)->get_children();
         int i=0;
         for(xmlpp::Element::NodeList::iterator switchIter = switchList.begin(); switchIter != switchList.end(); ++switchIter)
         {
             std::string nodeType=(*switchIter)->get_name();
             if(nodeType=="switch")
             {
                 const xmlpp::Element* element3 = dynamic_cast<const xmlpp::Element*>(*switchIter);
                 double value = std::stof(element3->get_attribute_value("value"));
                 switches[i]->set_value(value);
                 i++;
             }
         }
     }
 }
 std::setlocale(LC_ALL, origLocale); //Restore locale to the original state
} //end parcefile()

void load_patch() //Choose file to load 
{
    GtkWidget *dialog;
    std::string filename;
    dialog = gtk_file_chooser_dialog_new (
        "Load Patch",
        (GtkWindow*)(mainwindow),
        GTK_FILE_CHOOSER_ACTION_OPEN,
        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
        GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
        NULL);

    gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), ""); //Default directory
    gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog), "*.xml");
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        parcefile(filename);
        g_free (filename);
    }
    gtk_widget_destroy (dialog);
 } //end load_patch()

} //namespace Engine
