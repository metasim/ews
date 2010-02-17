project(EWS)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_BUILD_TYPE Debug)

find_package(OpenSceneGraph REQUIRED 
    COMPONENTS osg osgDB osgUtil osgViewer osgGA)
    
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})

include_directories (
	${CMAKE_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${QT_QTCORE_INCLUDE_DIR}
	${QT_QTGUI_INCLUDE_DIR}
	${QT_QTOPENGL_INCLUDE_DIR}
)

# set(EWS_HDRS QOSGWidget.h)

set(EWS_SRCS main.cpp EWSMainWindow.cpp)

set(EWS_UIS EWSMainWindow.ui)
QT4_WRAP_UI(UIS ${EWS_UIS})

set(EWS_MOCS EWSMainWindow.h)
QT4_WRAP_CPP(MOCS ${EWS_MOCS})

set(EWS_RSCS Resources.qrc)
QT4_ADD_RESOURCES(RSCS ${EWS_RSCS})

add_definitions(-Wall)

IF(APPLE)
    include(BundleUtilities)
   SET(EXE_TYPE MACOSX_BUNDLE)
   FIND_LIBRARY(CARBON_LIBRARY Carbon)
   FIND_LIBRARY(APP_SERVICES_LIBRARY ApplicationServices )
   MARK_AS_ADVANCED (CARBON_LIBRARY
                     APP_SERVICES_LIBRARY)
   SET(EXTRA_LIBS ${CARBON_LIBRARY} ${APP_SERVICES_LIBRARY})
   set(MACOSX_BUNDLE_ICON_FILE images/MustardSeedIcon.icns)
   set(MACOSX_BUNDLE_INFO_STRING "NVIDIA Wave Interference Simulation")
   set(MACOSX_BUNDLE_SHORT_VERSION_STRING "Version 0.0")
   set(MACOSX_BUNDLE_BUNDLE_VERSION 0.0)
   set(MACOSX_BUNDLE_COPYRIGHT "2010 NVIDIA Foundation")
#   set(EXE_NAME "Educational Wave Simulation")
#   fixup_bundle(\"EWS.app\" \"\" \"images\")
#   add_custom_command(TARGET EWS POST_BUILD COMMAND 
ENDIF (APPLE)


add_executable(EWS ${EXE_TYPE} ${EWS_SRCS} ${EWS_HDRS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries(EWS 
    ${QT_QTCORE_LIBRARY} 
    ${QT_QTGUI_LIBRARY} 
    ${QT_QTOPENGL_LIBRARY} 
    ${OPENSCENEGRAPH_LIBRARIES} 
    ${EXTRA_LIBS}
)
