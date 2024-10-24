cmake --workflow --preset Debug
cmake --workflow --preset Release
idt.exe -p build/Debug -apply-fixits -inplace -header-dir=include dummy.h
idt.exe -p build/Release -apply-fixits -inplace -header-dir=include dummy.h

idt.exe -p build/Debug -apply-fixits -inplace -header-dir=../clang/include dummy.h
idt.exe -p build/Release -apply-fixits -inplace -header-dir=../clang/include dummy.h