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


#include "ModuleList.h"

#include "modules/v100.h"
#include "modules/v101.h"
#include "modules/v200.h"
#include "modules/v201.h"
#include "modules/v203.h"
#include "modules/v210.h"
#include "modules/v211.h"
#include "modules/v212.h"
#include "modules/v213.h"
#include "modules/v230.h"
#include "modules/v300.h"
#include "modules/v400.h"
#include "modules/v701.h"
#include "modules/v702.h"
#include "modules/v703.h"
#include "modules/v1001.h"
#include "modules/v1005.h"

Module* create_new_module_from_ID(int ID){
    switch(ID){
        case V100:
            return new v100();
        case V101:
            return new v101();
        case V200:
            return new v200();
        case V201:
            return new v201();
        case V203:
            return new v203();
        case V210:
            return new v210();
        case V211:
            return new v211();
        case V212:
            return new v212();
        case V213:
            return new v213();
        case V230:
            return new v230();
        case V300:
            return new v300();
        case V400:
            return new v400();
        case V701:
            return new v701();
        case V702:
            return new v702();
        case V703:
            return new v703();
        case V1001:
            return new v1001();
        case V1005:
            return new v1005();
        default:
            return NULL;
    }
}
