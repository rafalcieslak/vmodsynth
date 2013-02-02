/*
 * This file is a part of vModSynth, (c) Rafał Cieślak 2012, 2013
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
