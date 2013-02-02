/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
 */

#include <cmath>
#include "algorithms.h"

double db2rms(double db){
    return exp((2.302585092994 * 0.05) * (db - 100.0));
}
