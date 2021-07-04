@echo off
call E:\VS2019\VC\Auxiliary\Build\vcvarsall x64

pushd bintmp
cl /c /MTd ../utils/unittest.cpp /I ../src
cl /c /MTd ../src/core/memory/globals.cpp /I ../src
cl /c  /MTd ../src/core/thread/mutex.cpp /I ../src
cl /c /MTd ../src/core/error/error.cpp /I ../src
popd

pushd bin64

lib /OUT:"libcrown.lib"  ../bintmp/globals.obj ../bintmp/mutex.obj ../bintmp/error.obj
link ../bintmp/unittest.obj ../lib/x64/Debug/libcrown.lib
popd

pause