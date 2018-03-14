mkdir build
cd build
mkdir x64
cd x64
cmake -DQT5_DIR=D:/dev/Qt/5.10.0/msvc2015_64/lib/cmake -DCMAKE_PREFIX_PATH="D:/DEV/git/pl_all/pl_3rdparty/package" -G "Visual Studio 14 Win64" ../../..
REM cmake --build . --config Release

PAUSE