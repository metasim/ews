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
#include "PointSampler.h"

namespace ews {
    namespace app {
        namespace model {
            
            SimulationState::SimulationState(QObject * parent) 
            : QObject(parent), _waveMedium(128, 128, 1, this),
            _dripSource1(_waveMedium.getWaveModel(), this), 
            _dripSource2(_waveMedium.getWaveModel(), this), 
            _barriers(this) {
                // We need to forward/create objectAdded events
                // from the BarrierSet.
                
                connect(&_barriers, SIGNAL(barrierAdded(int,Barrier*)),
                        SLOT(forwardBarrierSetAddition(int, Barrier*)));
                connect(&_barriers, SIGNAL(barrierRemoved(int,Barrier*)),
                        SLOT(forwardBarrierSetRemoval(int, Barrier*)));
                
                
                _dripSource1.setObjectName(ews::app::model::DRIPSOURCE1);
                _dripSource1.reset();
                
                _dripSource2.setObjectName(ews::app::model::DRIPSOURCE2);
                _dripSource2.reset();
                
                _waveMedium.setObjectName("waveMedium");
                setPaused(true);
            }
            
            SimulationState::~SimulationState()  {
                for (PointSamplerIterator i = _samplers.begin(); i != _samplers.end(); i++) {
                    PointSampler* s = *i;
                    delete s;
                }
            }
            
            void SimulationState::setPaused(bool state) {
                _dripSource1.setPaused(state);
                _dripSource2.setPaused(state);
                _waveMedium.setPaused(state);
                for (PointSamplerIterator pi = _samplers.begin(); pi != _samplers.end(); pi++) {
                    (*pi)->setPaused(state);
                }
            }
                                            
            void SimulationState::forwardBarrierSetAddition(int index, Barrier* barrier) {
                emit objectAdded(*barrier);
            }
            
            void SimulationState::forwardBarrierSetRemoval(int index, Barrier* barrier) {
                emit objectRemoved(*barrier);
            }
            
            void SimulationState::reset() {
                _waveMedium.getWaveModel().clear();
                _dripSource1.reset();
                _dripSource2.reset();
                _barriers.reset();
                
                // Hack to make sure cleared wave model gets rendered.
                if(isPaused()) {
                    _waveMedium.getWaveModel().propagate();
                }
            }
            
            PointSampler* SimulationState::createPointSampler() {
                WaveModel& model = _waveMedium.getWaveModel();
                PointSampler* retval = new PointSampler(model.getLattice(), this);
                retval->setPosition(Vec2(20, 100));
                _samplers.push_back(retval);
                emit objectAdded(*retval);
                return retval;
            }

        }
    }
}
