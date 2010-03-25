/* Copyright 2010 NVIDIA Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Developed by Mustard Seed Software, LLC 
 * http://mseedsoft.com
 */

#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>
#include <QString>
#include <QErrorMessage>
#include <osg/Notify>
#include <iostream>
#include <sstream>
#include "EWSMainWindow.h"

#if defined(WIN32) && !defined(__CYGWIN__)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define __WINLOG
#endif

inline void logMessage(const char* typeStr, const char* msg) {
    std::ostringstream oss;
    oss << typeStr << ": " << msg << std::endl;
    std::cerr << oss;
#if defined(__WINLOG)
    OutputDebugStringA(oss.str().c_str());
#endif
}

/** Message handler for Qt logging system. */
void messageHandler(QtMsgType type, const char *msg) {
    switch (type) {
        case QtDebugMsg:
            logMessage("Debug", msg);
            break;
        case QtWarningMsg:
            logMessage("Warning", msg);
            break;
        case QtCriticalMsg:
            logMessage("Critical", msg);
            break;
        case QtFatalMsg:
            logMessage("Fatal", msg);
            break;
    }
}


int main(int argc, char *argv[]) {
    using namespace ews::app::model;
    using namespace ews::app::widget;

#if defined(QT_DEBUG)                
    osg::setNotifyLevel(osg::INFO);
#endif                
    
    // To see Qt object dumps on macos, run with the environment variable
    // "DYLD_IMAGE_SUFFIX" set to "_debug".
    
    QApplication::setOrganizationName("Mustard Seed Software, LLC");
    QApplication::setOrganizationDomain("com.mseedsoft");
    
    qInstallMsgHandler(messageHandler);
    QApplication a(argc, argv);
    // QErrorMessage::qtHandler();
    a.setQuitOnLastWindowClosed(true);
    
    QSplashScreen splash;
    QPixmap img(":/images/splash.png");
    
    if(img.isNull()) {
       qWarning() << "Couldn't load splash image";
    }
    
    splash.setPixmap(img);
    splash.showMessage(QObject::tr("Starting up...."));
    splash.show();
    

    QApplication::setWindowIcon(QIcon(":/images/appicon"));
                           
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );

    SimulationState state;
    state.setObjectName("root");
    
    EWSMainWindow w(&state);

    w.show();
    
    splash.finish(&w);

    return a.exec();
}
