#--------------------------------------------------------------------------------
# This is the cmake project file for EWS, derived from the code from
# http://www.itk.org/Wiki/BundleUtilitiesExample
#
# To have CMake pick a Qt installation of your choice that won't be found
# automatically, set the CMAKE_PREFIX_PATH environment variable.
# For example: "export CMAKE_PREFIX_PATH=/usr/local/trolltech/qt4.3.5"
#  

PROJECT (EWS)
set(EXE_TARGET_NAME "VisualizeSTEM-Wave")
set(APP_NAME "Visualize STEM - Wave")
set(VERSION "0.1")
set(SCRIPT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/BuildScripts)

cmake_minimum_required(VERSION 2.8)

option(TESTING "build unit tests and include 'test' target" ON)

set(EWS_MODULES app/widget app/drawable app/model physics geometry util test)

#--------------------------------------------------------------------------------
# This is the list of source files that make up the physics engine
#--------------------------------------------------------------------------------
file(GLOB EWS_PHYS_SRC physics/*.cpp)

#--------------------------------------------------------------------------------
# This is the list of source files that need to be compiled
#--------------------------------------------------------------------------------
file(GLOB EWS_SRCS main.cpp app/widget/*.cpp app/model/*.cpp app/drawable/*.cpp physics/*.cpp)

#--------------------------------------------------------------------------------
# Regular (non-qt) header files
#--------------------------------------------------------------------------------
file(GLOB EWS_HDRS physics/*.h util/*.h geometry/*.h EWSDefine.h)


#--------------------------------------------------------------------------------
# another list, this time it includes all header files that should be treated with moc
#--------------------------------------------------------------------------------
file(GLOB EWS_MOC_HDRS app/widget/*.h app/model/*.h app/drawable/*.h)
file(GLOB EWS_TEST_MOC_HDRS test/physics/*.h)
# The following is suppposed to add header files to the IDE without them having
# be be added to the target executable.


# Move the given file from the EWS_MOC_HDRS list to the EWS_HDRS list
macro(unmoc FILENAME)
    file(GLOB_RECURSE MOCKERY ${FILENAME})
    if(NOT MOCKERY)
        message(FATAL_ERROR "Couldn't find ${FILENAME} to unmoc.")    
    else()    
        list(REMOVE_ITEM EWS_MOC_HDRS ${MOCKERY})
        list(APPEND EWS_HDRS ${MOCKERY})
    endif()
endmacro()    

unmoc(DrawableFactory.h)
unmoc(CameraController.h)
unmoc(VertexHeightUpdater.h)
unmoc(PickHandler.h)
unmoc(Teapot.h)
unmoc(Knob.h)
unmoc(PotentialUpdater.h)
unmoc(BarrierDragger.h)
unmoc(WaterBoundaryDragConstraint.h)

#message("moc files: ${EWS_MOC_HDRS}")

set_source_files_properties(${EWS_HDRS} PROPERTIES HEADER_FILE_ONLY TRUE) 


#--------------------------------------------------------------------------------
# .ui files that are used in the project
#--------------------------------------------------------------------------------
file(GLOB EWS_UIS app/widget/*.ui)

#--------------------------------------------------------------------------------
# and finally any resource files. 
#--------------------------------------------------------------------------------
file(GLOB_RECURSE EWS_RCS *.qrc)

#--------------------------------------------------------------------------------
# Vertex and fragment sharders
#--------------------------------------------------------------------------------
file(GLOB EWS_SHADERS app/drawable/waves.vs app/drawable/waves.fs)

#--------------------------------------------------------------------------------
#--------------------------------------------------------------------------------
# This sets the windows build that will need the special winmain@16 call. Qt provides
#  this for us in the qtmain.lib file. Using this cmake code will ensure we have it
#  linked into our build. Not needed on Unix/OS X/Linux which is why we have the 
#  if(WIN32) conditional.
if (WIN32)
    set (QT_USE_QTMAIN TRUE)
endif (WIN32)

#--------------------------------------------------------------------------------
# this command finds Qt4 libraries and sets all required variables
# note that it's Qt4, not QT4 or qt4
find_package( Qt4 REQUIRED )

#--------------------------------------------------------------------------------
# QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to
# a CMake script that sets up this directory for building with Qt4
include( ${QT_USE_FILE} )

#--------------------------------------------------------------------------------
# this command will generate rules that will run rcc on all files from EWS_RCS
# in result EWS_GEN_RCS variable will contain paths to files produced by rcc
qt4_add_resources( EWS_GEN_RCS ${EWS_RCS} )
foreach(SHADE_SRC ${SHADERS})
    set_source_files_properties(${EWS_GEN_RCS} PROPERTIES OBJECT_DEPENDS ${SHADE_SRC})
endforeach()

#--------------------------------------------------------------------------------
# this will run uic on .ui files:
qt4_wrap_ui( EWS_GEN_UIS ${EWS_UIS} )

#--------------------------------------------------------------------------------
# and finally this will run moc on the headers
qt4_wrap_cpp( EWS_GEN_MOC_HDRS ${EWS_MOC_HDRS} )
qt4_wrap_cpp( EWS_TEST_GEN_MOC_HDRS ${EWS_TEST_MOC_HDRS} )

#--------------------------------------------------------------------------------
# Add OpenSceneGraph suppport
find_package(OpenSceneGraph REQUIRED 
    COMPONENTS osg osgUtil osgViewer osgGA osgManipulator osgDB)

   
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)

#--------------------------------------------------------------------------------
# Make application compabilble with MacOS X Leopard.
if(APPLE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -isysroot /Developer/SDKs/MacOSX10.5.sdk")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isysroot /Developer/SDKs/MacOSX10.5.sdk")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmacosx-version-min=10.5")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmacosx-version-min=10.5")

    if(${CMAKE_GENERATOR} STREQUAL "Xcode")
        set(CMAKE_OSX_ARCHITECTURES "i386;")
    else()
        # Build for single architecture. Using this flag requires more than one
        # architecture in the list, so this hack of only providing one forces
        # a single architecture compile.
        set(CMAKE_OSX_ARCHITECTURES "i386;i386")
    endif()
endif()    

#--------------------------------------------------------------------------------
# we need this to be able to include headers produced by uic in our code
# CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES()
#  works just like INCLUDEPATH from qmake
include_directories (
  # Look in the cmake build directory (some generated headers could be there)
  ${CMAKE_CURRENT_BINARY_DIR} 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${EWS_MODULES}
)



#--------------------------------------------------------------------------------
# For Apple set the icns file containing icons
if(APPLE)
  set(ICON_NAME ews-icon.icns)
  # Set the executable wrapper type
  set(EXE_TYPE MACOSX_BUNDLE)
  # set how it shows up in the Info.plist file
  set(MACOSX_BUNDLE_ICON_FILE ${ICON_NAME}) 
  # set where in the bundle to put the icns file
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/images/${ICON_NAME} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  # include the icns file in the target
  set(EWS_SRCS ${EWS_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/images/${ICON_NAME})
  # Set additional application metadata.
  set(MACOSX_BUNDLE_BUNDLE_NAME ${APP_NAME})
  set(MACOSX_BUNDLE_IDENTIFIER com.mseedsoft.ews) # Doesn't seem to work in cmake 2.6
  set(MACOSX_BUNDLE_INFO_STRING "${APP_NAME} - Version ${VERSION}")
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${VERSION})
  set(MACOSX_BUNDLE_BUNDLE_VERSION ${VERSION})
  set(MACOSX_BUNDLE_COPYRIGHT "2010 NVIDIA Foundation")  
elseif(WIN32)
  # Set the executable wrapper type
  set(EXE_TYPE WIN32)
endif()

if(CMAKE_GENERATOR STREQUAL "Xcode")
    source_group("Headers" FILES ${EWS_HDRS})
    source_group("Headers (Qt MOC)"  FILES ${EWS_MOC_HDRS})
    source_group("Source (Qt Designer)" REGULAR_EXPRESSION ".*\\.ui$")
    source_group("Source (Generated)" FILES ${EWS_GEN_MOC_HDRS} ${EWS_GEN_UIS} ${EWS_GEN_RCS})
    source_group("Shaders" FILES ${EWS_SHADERS})
endif()


#--------------------------------------------------------------------------------
#  CMake's way of creating an executable
add_executable( ${EXE_TARGET_NAME} ${EXE_TYPE} 
  ${EWS_SRCS}
  ${EWS_GEN_MOC_HDRS} 
  ${EWS_GEN_UIS}
  ${EWS_GEN_RCS}
  ${EWS_HDRS}
)


# If we are build a "Debug" version then put that on the built Application
set(EXE_DEBUG_EXTENSION "_debug")

#-- Set the Debug and Release names for the libraries
set_target_properties(${EXE_TARGET_NAME} 
    PROPERTIES
    DEBUG_OUTPUT_NAME ${APP_NAME}${EXE_DEBUG_EXTENSION}
    RELEASE_OUTPUT_NAME ${APP_NAME}
)

# This shouldn't be necessary, but bits aren't getting compiled when the UI files are updated.
add_dependencies(${EXE_TARGET_NAME}  ${EWS_MOC_HDRS} ${EWS_HDRS} ${EWS_UIS} ${EWS_RCS} ${EWS_SHADERS})

#--------------------------------------------------------------------------------
# Tell CMake which libraries we need to link our executable against.
target_link_libraries (${EXE_TARGET_NAME}
    ${QT_LIBRARIES}
    ${QT_QTCORE_LIBRARY} 
    ${QT_QTGUI_LIBRARY} 
    ${QT_QTOPENGL_LIBRARY} 
    ${OPENTHREADS_LIBRARY}
    ${OPENSCENEGRAPH_LIBRARIES} 
    ${OPENGL_LIBRARIES}
)


#--------------------------------------------------------------------------------
#--------------------------------------------------------------------------------
# Now the installation stuff below
#--------------------------------------------------------------------------------
set(APP_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/bin/${EXE_TARGET_NAME}")
SET(QT_CONF_DEST "bin")
set(QT_PLUGIN_DEST "bin")
if(APPLE)
    set(CMAKE_INSTALL_PREFIX "/Applications")
    set(APP_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/${EXE_TARGET_NAME}.app")
    set(QT_PLUGIN_DEST "${EXE_TARGET_NAME}.app/Contents/MacOS/plugins")  
    set(QT_CONF_DEST "${EXE_TARGET_NAME}.app/Contents/Resources")  
elseif(WIN32)
    set(APP_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/bin/${EXE_TARGET_NAME}.exe")
endif()

#--------------------------------------------------------------------------------
# Install the EWS application, on Apple, the bundle is at the root of the
# install tree, and on other platforms it'll go into the bin directory.

install(TARGETS ${EXE_TARGET_NAME} 
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime
)

#--------------------------------------------------------------------------------
# Install needed Qt plugins by copying directories from the qt installation
# One can cull what gets copied by using 'REGEX "..." EXCLUDE' 
# NB: this is important because icon loading, etc. need image plugins.
install(DIRECTORY "${QT_PLUGINS_DIR}/imageformats" DESTINATION ${QT_PLUGIN_DEST} COMPONENT Runtime)


#--------------------------------------------------------------------------------
# install a qt.conf file
# this inserts some cmake code into the install script to write the file
# This is needed so Qt can find search path for plugins.
INSTALL(CODE "
    message(\"Creating \${CMAKE_INSTALL_PREFIX}/${QT_CONF_DEST}/qt.conf\")
    file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${QT_CONF_DEST}/qt.conf\" \"\")
    " COMPONENT Runtime)
    

if(APPLE)
    #--------------------------------------------------------------------------------
    # Use BundleUtilities to get all other dependencies for the application to work.
    # It takes a bundle or executable along with possible plugins and inspects it
    # for dependencies.  


    # directories to look for dependencies
    get_filename_component(OSG_LIBRARY_DIR ${OSG_LIBRARY} PATH)
    set(FIXUP_LIBRARY_SEARCH_PATH ${QT_LIBRARY_DIR} ${OSG_LIBRARY_DIR} ${QT_PLUGINS_DIR}/imageformats)
    ##message("FIXUP_LIBRARY_SEARCH_PATH = ${FIXUP_LIBRARY_SEARCH_PATH}")

    # Now the work of copying dependencies into the bundle/package
    # The quotes are escaped and variables to use at install time have their $ escaped
    # An alternative is the do a configure_file() on a script and use install(SCRIPT  ...).
    # over.
    install(CODE "
        include(InstallRequiredSystemLibraries)
        file(GLOB_RECURSE QTPLUGINS
            \"\${CMAKE_INSTALL_PREFIX}/${QT_PLUGIN_DEST}/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
        message(\"plugins: \${QTPLUGINS}\")
        set(FIXUP_EXTRA_LIBS ${OPENSCENEGRAPH_LIBRARIES};\${QTPLUGINS})
        include(BundleUtilities)
        fixup_bundle(\"${APP_INSTALL_PATH}\" \"\${FIXUP_EXTRA_LIBS}\" \"${FIXUP_LIBRARY_SEARCH_PATH}\")
        " COMPONENT Runtime)
endif()

    

if(WIN32)
    set(CPACK_GENERATOR ZIP)
    set(CPACK_SOURCE_GENERATOR ZIP)
else()
    set(CPACK_GENERATOR DMG)
    set(CPACK_SOURCE_GENERATOR DMG)
    if(APPLE)
        # To Create a package, one can run "cpack -G DragNDrop CPackConfig.cmake" on Mac OS X
        # where CPackConfig.cmake is created by including CPack
        # And then there's ways to customize this as well
        set(CPACK_BINARY_DRAGNDROP ON)
    endif()
endif()
include(CPack)




############################################################
# C++ tests built with standard compiler flags.

# cxx_test(name lib srcs...)
#
# creates a named test target that is built from the given source files.
function(cxx_test name path)
  add_executable(${name}
    ${path}/moc_${name}.cxx
    ${ARGN}
    ${EWS_HDRS}
  )
  add_dependencies(${name}
    ${ARGN}
    ${EWS_GEN_MOC_HDRS} 
    ${EWS_TEST_GEN_MOC_HDRS} 
    ${EWS_HDRS}
  )
  set_target_properties(${name}
    PROPERTIES
    COMPILE_FLAGS "${cxx_default}")
# Not all tests will need all libraries
  target_link_libraries(${name}
    ${QT_QTCORE_LIBRARY} 
    ${QT_QTGUI_LIBRARY} 
    ${QT_QTOPENGL_LIBRARY} 
    ${OPENSCENEGRAPH_LIBRARIES} 
    ${OPENGL_LIBRARIES}
    ${QT_QTTEST_LIBRARY}
  )
#  add_test(${name} ${name} -xml -o ${name}.xml)
  add_test(${name} ${name} -xunitxml -o ${name}.xml)
endfunction()

if(TESTING)
    enable_testing()

    # First argument is test/executable name, second argument is path to header, remaining arguments are required .cpp files
    cxx_test(LatticeTest test/physics test/physics/LatticeTest.cpp physics/Lattice.cpp)
    cxx_test(ConstantPotentialTest test/physics test/physics/ConstantPotentialTest.cpp)
    cxx_test(Point2dTest test/physics test/physics/Point2dTest.cpp)
    cxx_test(Vector2dTest test/physics test/physics/Vector2dTest.cpp)
    cxx_test(Line2dTest test/physics test/physics/Line2dTest.cpp)
    cxx_test(WallPotentialTest test/physics test/physics/WallPotentialTest.cpp physics/WallPotential.cpp)
    cxx_test(SlitPotentialTest test/physics test/physics/SlitPotentialTest.cpp physics/SlitPotential.cpp physics/WallPotential.cpp)
    cxx_test(CompositePotentialTest test/physics test/physics/CompositePotentialTest.cpp physics/CompositePotential.cpp physics/WallPotential.cpp)
    cxx_test(PrecomputedPotentialTest test/physics test/physics/PrecomputedPotentialTest.cpp physics/PrecomputedPotential.cpp physics/CompositePotential.cpp physics/WallPotential.cpp)
    cxx_test(DampedClassicalWavePropagatorTest test/physics test/physics/DampedClassicalWavePropagatorTest.cpp ${EWS_PHYS_SRC})
    cxx_test(WaveModelTest test/physics test/physics/WaveModelTest.cpp ${EWS_PHYS_SRC})
    cxx_test(OscillatorTest test/physics test/physics/OscillatorTest.cpp ${EWS_PHYS_SRC})
endif()
