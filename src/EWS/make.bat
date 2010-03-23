@echo off

set OSG_DIR=c:\OpenSceneGraph-2.8.2

if "%1"=="vs" goto VS
if "%1" == "debug" goto DEBUG

goto NMAKE


:NMAKE
set BDIR=build
if EXIST %BDIR% rmdir /s /q %BDIR%
mkdir %BDIR%
cd %BDIR%

cmake -G"NMake Makefiles" .. -DTESTING:BOOL=FALSE -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%CD% -DOSG_DIR:PATH=%OSG_DIR%
nmake
nmake package 
GOTO FINISH

:DEBUG
set BDIR=build.debug
if EXIST %BDIR% rmdir /s /q %BDIR%
mkdir %BDIR%
cd %BDIR%
cmake -G"NMake Makefiles" .. -DTESTING:BOOL=FALSE -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%CD% -DOSG_DIR:PATH=%OSG_DIR%
nmake
nmake package 
GOTO FINISH


:VS
set BDIR=build.vs
if EXIST %BDIR% rmdir /s /q %BDIR%
mkdir %BDIR%
cd %BDIR%
cmake -G"Visual Studio 9 2008" .. -DTESTING:BOOL=FALSE -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=%CD% -DOSG_DIR:PATH=%OSG_DIR%
start VisualizeSTEM-Wave.sln
cd ..
GOTO FINISH

:FINISH
echo Done.

