mkdir build
cd build
mkdir x64
cd x64
cmake -DQT5_DIR=D:/dev/Qt/5.9.1/msvc2015_64/lib/cmake -DCMAKE_PREFIX_PATH="D:/DEV/git/pl_all/pl_3rdparty/package/opencv-3.1.0/build" -G "Visual Studio 14 Win64" ../../..
cmake --build . --config Release

PAUSE