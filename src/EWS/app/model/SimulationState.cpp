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

#include "SimulationState.h"
#include <osg/Vec2>
namespace ews {
    namespace app {
        namespace model {
            SimulationState::SimulationState(QObject * parent) :
            QObject(parent), _waveMedium(128, 128, 1, this) ,_dripSource1(_waveMedium.getWaveModel(), this), 
            _dripSource2(_waveMedium.getWaveModel(), this) {
                _dripSource1.setObjectName("dripSource1");
                _dripSource1.setPosition(osg::Vec2(_waveMedium.getWidth()/2,_waveMedium.getLength()/2));
                _dripSource1.setEnabled(true);
                
                _dripSource2.setObjectName("dripSource2");
                _dripSource2.setPosition(osg::Vec2(20, 20));
                _dripSource2.setEnabled(false);
                
                _waveMedium.setObjectName("waveMedium");
                setPaused(true);
            }
            
            SimulationState::~SimulationState()  {
            }
            
            void SimulationState::setPaused(bool state) {
                _dripSource1.setPaused(state);
                _dripSource2.setPaused(state);
                _waveMedium.setPaused(state);
            }
            
            void SimulationState::reset() {
                _waveMedium.getWaveModel().clear();
            }
        }
    }
}