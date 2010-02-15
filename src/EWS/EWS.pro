# -------------------------------------------------
# Project created by QtCreator 2010-02-13T11:13:14
# -------------------------------------------------
QT += opengl
TARGET = EWS
TEMPLATE = app
SOURCES += main.cpp \
    EWSMainWindow.cpp \
    QOSGWidget.cpp

HEADERS += EWSMainWindow.h \
    QOSGWidget.h
FORMS += EWSMainWindow.ui \
    QOSGWidget.ui
macx:LIBS += -framework \
    osg \
    -framework \
    osgUtil \
    -framework \
    osgViewer -framework osgGA
