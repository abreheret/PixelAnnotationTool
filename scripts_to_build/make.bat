mkdir build
cd build
mkdir x64
cd x64

cmake -DQT5_DIR=[QtPath]/5.12.10/msvc2015_64/lib/cmake -DCMAKE_PREFIX_PATH=[OpenCVPath]/opencv-4.5.2/build -G "Visual Studio 14 Win64" ../../..

cmake --build . --config Release
cmake --build . --config Release --target make_zip

PAUSE