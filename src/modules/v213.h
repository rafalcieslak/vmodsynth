/*
    Copyright (C) 2016 Robert Gyllenberg

    extends Module class by Rafał Cieślak 2012, 2013

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


#ifndef V213_H
#define V213_H

#include "../Module.h"
#include <iostream>

/*
 * Random output voltage generator
 */
class v213 : public Module
{
    public:
        v213();
        virtual ~v213();
        void dsp();
        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
    private:
        double fRand(double fMin, double fMax);
        long cnt_time;
        double last_voltage;
        std::vector<int> scale_5;
        std::vector<int> scale_7;
        int last_note;
};

#endif // V213_H
