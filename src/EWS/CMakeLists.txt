#--------------------------------------------------------------------------------
# This is the cmake project file for EWS, derived from the code from
# http://www.itk.org/Wiki/BundleUtilitiesExample
#
# To have CMake pick a Qt installation of your choice that won't be found
# automatically, set the CMAKE_PREFIX_PATH environment variable.
# For example: "export CMAKE_PREFIX_PATH=/usr/local/trolltech/qt4.3.5"
#  

PROJECT (EWS)
set(EXE_NAME "Visualize STEM - Wave")
cmake_minimum_required(VERSION 2.8)

#add_definitions(-fdiagnostics-show-option)

#--------------------------------------------------------------------------------
# This is the list of source files that need to be compiled
#--------------------------------------------------------------------------------
file(GLOB EWS_SRCS main.cpp ui/*.cpp model/*.cpp drawable/*.cpp physics/*.cpp)

#--------------------------------------------------------------------------------
# Regular (non-qt) header files
#--------------------------------------------------------------------------------
file(GLOB EWS_HDRS drawable/*.h physics/*.h)

#--------------------------------------------------------------------------------
# another list, this time it includes all header files that should be treated with moc
#--------------------------------------------------------------------------------
file(GLOB EWS_MOC_HDRS ui/*.h model/*.h drawable/EWSDrawable.h)

#--------------------------------------------------------------------------------
# .ui files that are used in the project
#--------------------------------------------------------------------------------
file(GLOB EWS_UIS ui/*.ui)

#--------------------------------------------------------------------------------
# and finally any resource files. 
#--------------------------------------------------------------------------------
file(GLOB_RECURSE EWS_RCS *.qrc)

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

#--------------------------------------------------------------------------------
# this will run uic on .ui files:
qt4_wrap_ui( EWS_GEN_UIS ${EWS_UIS} )

#--------------------------------------------------------------------------------
# and finally this will run moc on the headers
qt4_wrap_cpp( EWS_GEN_MOC_HDRS ${EWS_MOC_HDRS} )

#--------------------------------------------------------------------------------
# Add OpenSceneGraph suppport
find_package(OpenSceneGraph REQUIRED 
    COMPONENTS osg osgUtil osgViewer osgGA)
    
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

#--------------------------------------------------------------------------------
# this helps the system find gtest
include(FindGTest)
#if(APPLE)
#    include_directories(/Developer/Library/Frameworks/gtest.framework/Headers)
#    find_library(GTEST_LIBRARY gtest)
#    mark_as_advanced(GTEST_LIBRARY)
#endif()

#--------------------------------------------------------------------------------
# this fixes 64-bit problems on Snow Leopard
if(APPLE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -isysroot /Developer/SDKs/MacOSX10.5.sdk")
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -isysroot /Developer/SDKs/MacOSX10.5.sdk")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmacosx-version-min=10.5")
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -mmacosx-version-min=10.5")
    #Option -arch is only available with apple compilers
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-arch i386" HAS_ARCH_FLAG)
#    if(HAS_ARCH_FLAG)
        if(${CMAKE_GENERATOR} STREQUAL "Xcode")
            set(CMAKE_OSX_ARCHITECTURES "i386;")
        else()
            # Build for single architecture. Using this flag requires more than one
            # architecture in the list, so this hack of only providing one forces
            # a single architecture compile.
            set(CMAKE_OSX_ARCHITECTURES "i386;i386")
        endif()
#    elseif(NOT $ENV{M64})
#	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
#	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
#    else()
#	message("Using 64-bit mode on Mac")
#    endif()
endif()    

find_package(OpenGL REQUIRED)


#--------------------------------------------------------------------------------
# we need this to be able to include headers produced by uic in our code
# CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES()
#  works just like INCLUDEPATH from qmake
include_directories (
  # Look in the cmake build directory (some generated headers could be there)
  ${CMAKE_CURRENT_BINARY_DIR} 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ui drawable model physics
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
  set(MACOSX_BUNDLE_BUNDLE_NAME ${EXE_NAME})
  set(MACOSX_BUNDLE_IDENTIFIER com.mseedsoft.ews) # Doesn't seem to work in cmake 2.6
  set(MACOSX_BUNDLE_INFO_STRING "NVIDIA Wave Interference Simulation")
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING 0.1)
  set(MACOSX_BUNDLE_BUNDLE_VERSION 0.1)
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
endif()


#--------------------------------------------------------------------------------
#  CMake's way of creating an executable
add_executable( ${EXE_NAME} ${EXE_TYPE} 
  ${EWS_SRCS}
  ${EWS_GEN_MOC_HDRS} 
  ${EWS_GEN_UIS}
  ${EWS_GEN_RCS}
  ${EWS_HDRS}
  ${EWS_MOC_HDRS}
)


set_property(
    TARGET ${EXE_NAME} 
    APPEND
    PROPERTY XCODE_ATTRIBUTE_DYLD_IMAGE_SUFFIX _debug
)

# This shouldn't be necessary, but bits aren't getting compiled when the UI files are updated.
add_dependencies(${EXE_NAME}  ${EWS_MOC_HDRS} ${EWS_HDRS} ${EWS_UIS} ${EWS_RCS})

#--------------------------------------------------------------------------------
# Tell CMake which libraries we need to link our executable against.
target_link_libraries ( ${EXE_NAME} 
    ${QT_QTCORE_LIBRARY} 
    ${QT_QTGUI_LIBRARY} 
    ${QT_QTOPENGL_LIBRARY} 
    ${OPENSCENEGRAPH_LIBRARIES} 
    ${OPENGL_LIBRARIES}
)




#--------------------------------------------------------------------------------
#--------------------------------------------------------------------------------
# Now the installation stuff below
#--------------------------------------------------------------------------------
set(plugin_dest_dir bin)
set(qtconf_dest_dir bin)
set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/\${EXE_NAME}")
if(APPLE)
  set(plugin_dest_dir ${EXE_NAME}.app/Contents/MacOS)
  set(qtconf_dest_dir ${EXE_NAME}.app/Contents/Resources)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/\${EXE_NAME}.app")
endif(APPLE)
if(WIN32)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/\${EXE_NAME}.exe")
endif(WIN32)

#--------------------------------------------------------------------------------
# Install the EWS application, on Apple, the bundle is at the root of the
# install tree, and on other platforms it'll go into the bin directory.
install(TARGETS ${EXE_NAME} 
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime
    )

#--------------------------------------------------------------------------------
# Install needed Qt plugins by copying directories from the qt installation
# One can cull what gets copied by using 'REGEX "..." EXCLUDE'
install(DIRECTORY "${QT_PLUGINS_DIR}/imageformats" DESTINATION ${plugin_dest_dir}/plugins COMPONENT Runtime)

#--------------------------------------------------------------------------------
# install a qt.conf file
# this inserts some cmake code into the install script to write the file
install(CODE "
    file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir}/qt.conf\" \"\")
    " COMPONENT Runtime)


#--------------------------------------------------------------------------------
# Use BundleUtilities to get all other dependencies for the application to work.
# It takes a bundle or executable along with possible plugins and inspects it
# for dependencies.  If they are not system dependencies, they are copied.

# directories to look for dependencies
set(DIRS ${QT_LIBRARY_DIRS})

# Now the work of copying dependencies into the bundle/package
# The quotes are escaped and variables to use at install time have their $ escaped
# An alternative is the do a configure_file() on a script and use install(SCRIPT  ...).
# Note that the image plugins depend on QtSvg and QtXml, and it got those copied
# over.
install(CODE "
    file(GLOB_RECURSE QTPLUGINS
      \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
    include(BundleUtilities)
    fixup_bundle(\"${APPS}\" \"\${QTPLUGINS}\" \"${DIRS}\")
    " COMPONENT Runtime)


# To Create a package, one can run "cpack -G DragNDrop CPackConfig.cmake" on Mac OS X
# where CPackConfig.cmake is created by including CPack
# And then there's ways to customize this as well
set(CPACK_BINARY_DRAGNDROP ON)
include(CPack)

############################################################
# C++ tests built with standard compiler flags.

# cxx_test(name lib srcs...)
#
# creates a named test target that is built from the given source files.
function(cxx_test name)
  add_executable(${name} ${ARGN} ${EWS_HDRS})
  add_dependencies(${name} ${ARGN} ${EWS_HDRS})
  set_target_properties(${name}
    PROPERTIES
    COMPILE_FLAGS "${cxx_default}")
  target_link_libraries(${name} ${GTEST_LIBRARY})
  add_test(${name} ${name})
endfunction()

cxx_test(LatticeTest test/physics/LatticeTest.cpp physics/Lattice.cpp)
