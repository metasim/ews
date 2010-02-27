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

#include "SimulationState.h"
#include "SceneRoot.h"



namespace ews {
    namespace app {
        namespace widget {
            
            /** Forward declaration of generated UI class. */
            namespace Ui {
                class EWSMainWindow;
            }
            
            using ews::app::model::SimulationState;
            using ews::app::drawable::SceneRoot;
            class EWSMainWindow : public QMainWindow {
                Q_OBJECT
                
            public:
                EWSMainWindow(SimulationState* state, QWidget *parent = 0);
                virtual ~EWSMainWindow();

                
            private:
                Q_DISABLE_COPY(EWSMainWindow)
                Ui::EWSMainWindow* _ui;
                SimulationState* _state;
                SceneRoot* _sceneRoot;
            };
        }
    }
}

#endif // EWSMAINWINDOW_H
