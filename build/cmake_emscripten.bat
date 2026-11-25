@echo off
emcmake cmake -S "./../" -B build_web -DCMAKE_INSTALL_PREFIX="SDK" %* && cmake --build build_web -j4
echo Run cmake --build build_web -j4