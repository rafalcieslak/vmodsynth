vModSynth
===
a modular software synthesizer
by Rafał Cieślak, 2012-2013
Distributed on GNU GPL v3 license, see LICENSE file for details.

To run vModSynth you need to have gtkmm-3.0 and ALSA run-time libriries installed on your system.
To compile it, you will also need the developement headers of these libraries.

To compile:
<pre>
	autoreconf
	./configure
	make
</pre>	

To install (as root):
<pre>
	make install
</pre>
	
To run:
<pre>
	vModSynth
</pre>
	
(or `src/vModSynth` if you didn't install it).

For detailed documentation, please refer to manual provided within the `./doc` directory.

I hope you will find vModSynth usefull.

Rafał Cieślak, 2012-2013