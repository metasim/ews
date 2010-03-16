rmdir /s /q build.vs
mkdir build.vs
REM copy ..\..\..\..\OpenSceneGraph-2.8.2\bin\osg55-osgd.dll build.vs\osg55-osg.dll
cd build.vs
cmake -G"Visual Studio 9 2008" ..
cd ..
