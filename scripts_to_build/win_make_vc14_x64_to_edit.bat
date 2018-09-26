mkdir build
cd build
mkdir x64
cd x64

cmake -DQT5_DIR=D:/Qt/5.11.2/msvc2015_64/lib/cmake -DCMAKE_PREFIX_PATH=D:/lib/opencv/opencv343/build -G "Visual Studio 14 Win64" ../../..

cmake --build . --config Release

PAUSE