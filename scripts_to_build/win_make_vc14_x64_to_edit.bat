mkdir build
cd build
mkdir x64
cd x64

cmake -DQT5_DIR=/path/to/Qt/5.9.1/msvc2015_64/lib/cmake -DCMAKE_PREFIX_PATH=/path/to/OpenCV/build -G "Visual Studio 14 Win64" ../../..

cmake --build . --config Release

PAUSE