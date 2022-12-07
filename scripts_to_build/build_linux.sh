#!/bin/bash

if [ -d "build" ]; then
	rm -r build
fi
mkdir build
cd build
cmake -DQT5_DIR=[QtPath]/5.12.10/gcc_64/lib/cmake -G "Unix Makefiles" ../..
make
cd ..

