/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#include "ModuleList.h"

#include "modules/v100.h"
#include "modules/v101.h"
#include "modules/v200.h"
#include "modules/v201.h"
#include "modules/v203.h"
#include "modules/v210.h"
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
