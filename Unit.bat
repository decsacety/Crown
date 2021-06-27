@echo off
call E:\VS2019\VC\Auxiliary\Build\vcvarsall x64

pushd bintmp
cl /c ../utils/unittest.cpp /I ../src
popd

pushd bin64
link ../bintmp/unittest.obj 
popd

pause