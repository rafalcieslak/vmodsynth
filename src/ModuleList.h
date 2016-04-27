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
    V211 = 211,
    V212 = 212,
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
