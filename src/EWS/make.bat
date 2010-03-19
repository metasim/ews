if %1==vs goto VS

goto NMAKE


:NMAKE
rmdir /s /q build
mkdir build
cd build
cmake -G"NMake Makefiles" .. -DTESTING:BOOL=FALSE -DCMAKE_BUILD_TYPE=Release
nmake
nmake install 
GOTO FINISH


:VS
rmdir /s /q build.vs
mkdir build.vs
cd build.vs
cmake -G"Visual Studio 9 2008" .. -DTESTING:BOOL=FALSE -DCMAKE_BUILD_TYPE=Release
start VisualizeSTEM-Wave.sln
GOTO FINISH

:FINISH
cd ..

@echo off

REM rmdir /s /q build.vs
REM mkdir build.vs
REM mkdir build.vs\Debug
REM copy ..\..\..\..\OpenSceneGraph-2.8.2\bin\bin\*d.dll build.vs\Debug
REM mkdir build.vs\Release
REM copy ..\..\..\..\OpenSceneGraph-2.8.2\bin\bin\*.dll build.vs\Release
REM del /Q build.vs\Release\*d.dll
REM cd build.vs
REM cmake -G"Visual Studio 9 2008" ..
REM cd ..
