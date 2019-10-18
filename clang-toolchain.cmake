# Set clang compiler variables
#
# Use when calling cmake like this:
# cmake -DCMAKE_TOOLCHAIN_FILE=../clang-toolchain.cmake ../

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_AR      llvm-ar)
set(CMAKE_LINKER  llvm-ld)
set(CMAKE_NM      llvm-nm)
set(CMAKE_OBJDUMP llvm-objdump)
set(CMAKE_RANLIB  llvm-ranlib)
