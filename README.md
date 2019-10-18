# Building

    mkdir build
    cd build
    cmake ../
    make

## Cmake errors

If you get an error about gcc being too old try using clang:

    cmake -DCMAKE_TOOLCHAIN_FILE=../clang-toolchain.cmake ../
