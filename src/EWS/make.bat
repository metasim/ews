rmdir /s /q build.vs
mkdir build.vs
mkdir build.vs\Debug
copy ..\..\..\..\OpenSceneGraph-2.8.2\bin\bin\*d.dll build.vs\Debug
mkdir build.vs\Release
copy ..\..\..\..\OpenSceneGraph-2.8.2\bin\bin\*.dll build.vs\Release
del /Q build.vs\Release\*d.dll
cd build.vs
cmake -G"Visual Studio 9 2008" ..
cd ..
