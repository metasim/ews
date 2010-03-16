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

#ifndef EWSMAINWINDOW_H
#define EWSMAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtCore/QMap>
#include <osg/Node>
#include <osg/ref_ptr>
#include "SimulationState.h"
#include "SceneRoot.h"


/** @cond */
/** Forward declaration of generated UI class. */
namespace Ui {
    class EWSMainWindowForm;
}
/** @endcond */

namespace ews {
    namespace app {
        namespace widget {
            using ews::app::model::SimulationState;
            using ews::app::drawable::SceneRoot;
            
            // cond/endcond is used to tell Doxygen to ignore what's in between
            class EWSMainWindow : /** @cond */ public QMainWindow /** @endcond */ {
                Q_OBJECT
                
            public:
                explicit EWSMainWindow(SimulationState* state, QWidget *parent = 0);
                virtual ~EWSMainWindow();

            public slots:
                void start();
                void stop();
                void reset();
                
            private:
                Q_DISABLE_COPY(EWSMainWindow)
                Ui::EWSMainWindowForm* _ui;
                SimulationState* _state;
                osg::ref_ptr<SceneRoot> _sceneRoot;
            };
        }
    }
}

#endif // EWSMAINWINDOW_H
