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



#include "Barrier.h"
#include "BarrierSet.h"
#include "SlitPotential.h"
#include "PrecomputedPotential.h"
#include "SimulationState.h"
#include "WaveModel.h"
#include <osg/Vec2>
#include "EWSDebug.h"

namespace ews {
    namespace app {
        namespace model {
            using ews::physics::SlitPotential;
            using ews::physics::Potential;
            using ews::physics::PrecomputedPotential;
            
            Barrier::Barrier(BarrierSet* parent) 
            : QObject(parent), _enabled(true), _numSlits(TWO), 
            _slitWidth(3), _slitSeparation(5), _start(10, 10), _end(10, 50), _potential(NULL) {
                
                QObject::connect(this, SIGNAL(dataChanged()), this, SLOT(updatePotentials()));
                
                updatePotentials();
            }
            
            BarrierSet* Barrier::getBarrierSet() {
                QObject* obj = parent();
                return qobject_cast<BarrierSet*>(obj);
            }
            Barrier::~Barrier() {
//                if(_potential) {
//                    delete _potential;
//                }
            }
            void Barrier::updatePotentials() {
                QTRACE;
                
//                if(_potential) {
//                    delete _potential;
//                }
//                
                
                BarrierSet* barriers = getBarrierSet();
                if(!barriers) {
                    qWarning() << "Unexpected missing barrier set.";
                    return;
                }
                
                SimulationState* state = barriers->getSimulationState();
                if(!state) {
                    qWarning() << "Unexpected missing simulation state.";
                    return;
                }
                
                WaveModel& waveModel = state->getWaveMedium().getWaveModel();
                SlitPotential* sp = new SlitPotential(getStart(), getEnd(), getNumSlits());
                sp->setSlitWidth(getSlitWidth());
                
                counted_ptr<const Potential> p(sp);
                PrecomputedPotential* prePot = new PrecomputedPotential(p, waveModel.getWidth(),
                                                                        waveModel.getLength());
                counted_ptr<const Potential> ptrPrePot(prePot);
                waveModel.setPotential(ptrPrePot);
                
//                _potential  = prePot;
            }

            
        }
    }
}