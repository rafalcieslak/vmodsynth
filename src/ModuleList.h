/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#ifndef MODULE_LIST_H
#define MODULE_LIST_H

#include "Module.h"

enum ModuleList{
    V100 = 100,
    V101 = 101,
    V200 = 200,
    V201 = 201,
    V203 = 203,
    V210 = 210,
    V230 = 230,
    V300 = 300,
    V400 = 400,
    V701 = 701,
    V702 = 702,
    V703 = 703,
    V1001 = 1001,
    V1005 = 1005
};

Module* create_new_module_from_ID(int ID);

#endif // MODULE_LIST_H
