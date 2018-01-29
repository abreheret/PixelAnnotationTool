#!/bin/bash

if [ -d "build" ]; then
	rm -r build
fi
mkdir build
cd build
cmake -DQT5_DIR=/home/amaury/Qt5.9.1/5.9.1/gcc_64/lib/cmake -G"Unix Makefiles" ../..
make
cd ..

