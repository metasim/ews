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

#include "EWSMainWindow.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    QSplashScreen* splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/images/splash.png"));
    splash->showMessage(QObject::tr("Starting up...."));
    
    a.setQuitOnLastWindowClosed(true);
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    

    
    SimulationState state;
    state.setObjectName("root");
    
    // a.connect(state.dripSource1(), SIGNAL(enabledChanged(bool)), &a, SLOT(aboutQt()));
    
    state.dumpObjectTree();
    
    EWSMainWindow w(&state);

    w.show();
    
    splash->finish(&w);
    delete splash;
    
    return a.exec();
}
