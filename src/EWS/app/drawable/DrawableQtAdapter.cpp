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


#include "DrawableQtAdapter.h"
#include <osgManipulator/CommandManager>

namespace ews {
    namespace app {
        namespace drawable {
            DrawableQtAdapter::DrawableQtAdapter() 
            : QObject(NULL), osg::PositionAttitudeTransform() {
                
            }
            
            
            DrawableQtAdapter::~DrawableQtAdapter() {
                
            }
            
            osgManipulator::CommandManager& DrawableQtAdapter::manipCommandManager() {
                static osg::observer_ptr<osgManipulator::CommandManager> _manipCommander;
                if(!_manipCommander.valid()) {
                    _manipCommander = new osgManipulator::CommandManager;
                }
                
                return *(_manipCommander.get());
            }
        }
    }
}
