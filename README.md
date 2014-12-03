vModSynth
===
a modular software synthesizer
by Rafał Cieślak, 2012-2013
Distributed on GNU GPL v3 license, see LICENSE file for details.

To run vModSynth you need to have gtkmm-3.0 and ALSA run-time libriries installed on your system.
To compile it, you will also need the developement headers of these libraries.

To compile:

    aclocal
    automake --add-missing
    autoreconf
    ./configure
    make

To install (as root):

    make install

To run:

    vmodsynth

(or `./src/vmodsynth` if you didn't install it).

For detailed documentation, please refer to manual provided within the `./doc` directory.

I hope you will find vModSynth usefull.

Rafał Cieślak, 2012-2013
