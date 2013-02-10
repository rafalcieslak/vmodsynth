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


#ifndef V701_H
#define V701_H

#include "../Module.h"
#include <queue>

/*
 * Echo effect module
 */

class v701 : public Module
{
    public:
        v701();
        virtual ~v701();
        void dsp();
        void draw(const Cairo::RefPtr<Cairo::Context>& cr);
    protected:
    private:
        std::queue<double> del;

};

#endif // V701_H
