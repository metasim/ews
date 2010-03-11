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
            SimulationState::SimulationState(QObject * parent) 
            : QObject(parent), _waveMedium(128, 128, 2, this),
            _dripSource1(_waveMedium.getWaveModel(), this), 
            _dripSource2(_waveMedium.getWaveModel(), this), 
            _barriers(this) {
                // We need to forward/create objectAdded events
                // from the BarrierSet.
                
                QObject::connect(&_barriers, SIGNAL(barrierAdded(int,Barrier*)),
                                 this, SLOT(forwardBarrierSetAddition(int, Barrier*)));
                QObject::connect(&_barriers, SIGNAL(barrierRemoved(int,Barrier*)),
                                 this, SLOT(forwardBarrierSetRemoval(int, Barrier*)));
                
                
                _dripSource1.setObjectName(ews::app::model::DRIPSOURCE1);
                _dripSource1.initialize(_waveMedium.getWidth(), _waveMedium.getLength());
                
                _dripSource2.setObjectName(ews::app::model::DRIPSOURCE2);
                _dripSource2.initialize(_waveMedium.getWidth(), _waveMedium.getLength());
                
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
                                            
            void SimulationState::forwardBarrierSetAddition(int index, Barrier* barrier) {
                emit objectAdded(*barrier);
            }
            
            void SimulationState::forwardBarrierSetRemoval(int index, Barrier* barrier) {
                emit objectRemoved(*barrier);
            }
            
            void SimulationState::reset() {
                _waveMedium.getWaveModel().clear();
                _dripSource1.initialize(_waveMedium.getWidth(), _waveMedium.getLength());
                _dripSource2.initialize(_waveMedium.getWidth(), _waveMedium.getLength());
                
                if(isPaused()) {
                    _waveMedium.getWaveModel().propagate();
                }
            }
        }
    }
}