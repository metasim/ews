project( EWS )
cmake_minimum_required( VERSION 2.6 )
set ( CMAKE_BUILD_TYPE Release )
add_definitions ( -Wall )

find_package(OpenSceneGraph REQUIRED COMPONENTS 
    osgDB osgUtil osgViewer osgGA)
    
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

find_package ( Qt4 REQUIRED )

include ( ${QT_USE_FILE} )

include_directories (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTOPENGL_INCLUDE_DIR}
	)

set (EWS_HDRS QOSGWidget.h)

set (EWS_SRCS main.cpp 	EWSMainWindow.cpp 	QOSGWidget.cpp	)

set ( EWS_UIS
	EWSMainWindow.ui
	)
QT4_WRAP_UI(UIS ${EWS_UIS})

set ( EWS_MOCS
	EWSMainWindow.h

	)
QT4_WRAP_CPP(MOCS ${EWS_MOCS})


IF(APPLE)
   SET(GUI_TYPE MACOSX_BUNDLE)
   FIND_LIBRARY(CARBON_LIBRARY Carbon)
   FIND_LIBRARY(APP_SERVICES_LIBRARY ApplicationServices )
   MARK_AS_ADVANCED (CARBON_LIBRARY
                     QUICKTIME_LIBRARY
                     APP_SERVICES_LIBRARY)
   SET(EXTRA_LIBS ${CARBON_LIBRARY} ${QUICKTIME_LIBRARY} ${APP_SERVICES_LIBRARY})
ENDIF (APPLE)


add_executable ( EWS ${GUI_TYPE} ${EWS_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( EWS  ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTOPENGL_LIBRARY} ${OPENSCENEGRAPH_LIBRARIES} ${EXTRA_LIBS})
