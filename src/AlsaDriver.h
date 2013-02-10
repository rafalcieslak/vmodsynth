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

#ifndef ALSA_DRIVER_H
#define ALSA_DRIVER_H

namespace AlsaDriver{

void thread_main(char *device);
void add_sample(double l, double r);

int get_last_note_pitch(int ch);
int get_last_note_velocity(int ch);
int get_notes_on(int ch);

}
#endif // ALSA_DRIVER_H
