#!/bin/sh


rm -r build/
mkdir build
mkdir build/modules

for i in *.cpp
do
    echo "Compiling $i into build/$i.o ..."
    g++ `pkg-config --cflags gtkmm-3.0 cairomm-1.0` -std=c++11 -s -O3 -c -o build/$i.o $i
done

for i in modules/*.cpp
do
    echo "Compiling $i into build/$i.o ..."
    g++ `pkg-config --cflags gtkmm-3.0 cairomm-1.0` -std=c++11 -s -O3 -c -o build/$i.o $i
done

echo "Linking..."
g++ -std=c++11 -s -O3 -o vmodsynth `find build -name *.o` `pkg-config --cflags --libs gtkmm-3.0 cairomm-1.0` -lasound

echo "Done. Use ./vmodsynth to run."
