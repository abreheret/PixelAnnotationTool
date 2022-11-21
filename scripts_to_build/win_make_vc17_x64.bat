mkdir build
cd build
mkdir x64
cd x64

cmake -DQT5_DIR=C:/Qt/5.15.2/msvc2019_64/lib/cmake -DCMAKE_PREFIX_PATH=../opencv/build/install -G "Visual Studio 17 2022"  ../../..

cmake --build . --config Release

PAUSE