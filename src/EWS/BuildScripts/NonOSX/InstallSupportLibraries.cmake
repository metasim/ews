# ------------------------------------------------------------------------------ 
# This CMake code installs the needed support libraries on NON OSX platforms
# See http://www.vtk.org/Wiki/BuildingOSXApplications
# ------------------------------------------------------------------------------ 

INCLUDE (InstallRequiredSystemLibraries)

# ---------- Find/Install the needed Qt4 libraries. 
SET (QTLIBLIST QtCore QtGui QtOpenGL)
IF(NOT Q_WS_MAC)
  FOREACH(qtlib ${QTLIBLIST})
    IF(WIN32)
      GET_FILENAME_COMPONENT(QT_DLL_PATH_tmp ${QT_QMAKE_EXECUTABLE} PATH)
      INSTALL(FILES ${QT_DLL_PATH_tmp}/${qtlib}d4.dll 
          DESTINATION bin 
          CONFIGURATIONS Debug 
          COMPONENT Runtime)
      INSTALL(FILES ${QT_DLL_PATH_tmp}/${qtlib}4.dll 
          DESTINATION bin 
          CONFIGURATIONS Release 
          COMPONENT Runtime)
    ENDIF()
  ENDFOREACH()
ENDIF()

# ---------- Find/Install the needed Qt4 libraries. 
IF(WIN32)
  FOREACH(osglib ${OPENSCENEGRAPH_LIBRARIES})
    GET_FILENAME_COMPONENT(OSG_DLL_PATH_tmp ${OSG_LIBRARY} PATH)
    INSTALL(FILES ${OSG_DLL_PATH_tmp}/${osglib}d4.dll 
          DESTINATION bin 
          CONFIGURATIONS Debug 
          COMPONENT Runtime)
    INSTALL(FILES ${OSG_DLL_PATH_tmp}/${osglib}4.dll 
          DESTINATION bin 
          CONFIGURATIONS Release 
          COMPONENT Runtime)
  ENDFOREACH()
ENDIF()