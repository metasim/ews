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
//#include <QString>
#include <QErrorMessage>
#include <osg/Notify>
#include "EWSMainWindow.h"
#include "EWSVersion.h"
#include "ErrorWrapper.h"
using ews::app::widget::saveHandler;
using ews::app::widget::errHandler;

#if defined(WIN32) && !defined(__CYGWIN__)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define __WINLOG
#endif

/** Application launch functionn. */
int main(int argc, char *argv[]) {
    using namespace ews::app::model;
    using namespace ews::app::widget;

#if defined(QT_DEBUG)                
    osg::setNotifyLevel(osg::INFO);
#endif                
    
    // To see Qt object dumps on macos, run with the environment variable
    // "DYLD_IMAGE_SUFFIX" set to "_debug".
    
    QApplication mainQApp(argc, argv);
    mainQApp.setQuitOnLastWindowClosed(true);
    QApplication::setOrganizationName(EWS_ORGANIZATION_NAME);
    QApplication::setOrganizationDomain(EWS_BUNDLE_ID);
    QApplication::setApplicationVersion(EWS_VERSION);
    QApplication::setApplicationName(EWS_APP_NAME);
    
    // First register the dialog error handler, then
    // get the function pointer to it by passing zero to the handler installer
    // then install our own.
    QErrorMessage::qtHandler();
    saveHandler(qInstallMsgHandler(0));
    qInstallMsgHandler(errHandler);
    
    QSplashScreen splash;
    QPixmap img(":/images/splash");
    
    if (img.isNull()) {
       qWarning() << "Couldn't load splash image";
    }
    
    splash.setPixmap(img);
    splash.showMessage(QObject::tr("Starting up...."));
    splash.show();
    
    mainQApp.connect(&mainQApp, SIGNAL(lastWindowClosed()), &mainQApp, SLOT(quit()));

    SimulationState state;
    state.setObjectName("root");
    
    EWSMainWindow w(&state);
    w.setWindowTitle(EWS_APP_NAME);
#if defined(__APPLE__)
    w.setWindowIcon(QIcon(":/images/appicon"));
#else    
    QApplication::setWindowIcon(QIcon(":/images/appicon"));    
#endif    


    w.show();
    
    splash.finish(&w);

    return mainQApp.exec();
}
