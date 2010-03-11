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
#include "PrecomputedPotential.h"
#include "SimulationState.h"
#include "WaveModel.h"
#include <osg/Vec2>
#include <osg/ref_ptr>
#include "EWSDebug.h"

namespace ews {
    namespace app {
        /**
         * Contains classes responsible for business logic associated with drawable objects.
         */
        namespace model {
            using ews::physics::SlitPotential;
            using ews::physics::Potential;
            using ews::physics::PrecomputedPotential;
            
            Barrier::Barrier(BarrierSet* parent) 
            : QObject(parent), _enabled(true), _numSlits(TWO), 
            _slitWidth(3), _slitSeparation(5), _start(10, 10), _end(10, 50), _potential(NULL) {                
            }
            
            BarrierSet* Barrier::getBarrierSet() {
                QObject* obj = parent();
                return qobject_cast<BarrierSet*>(obj);
            }
            Barrier::~Barrier() {
            
            }
            
            ref_ptr<Potential> Barrier::generatePotential() {

                BarrierSet* barriers = getBarrierSet();
                if(!barriers) {
                    qWarning() << "Unexpected missing barrier set.";
                    return ref_ptr<Potential>(NULL);
                }
                
                SimulationState* state = barriers->getSimulationState();
                if(!state) {
                    qWarning() << "Unexpected missing simulation state.";
                    return ref_ptr<Potential>(NULL);
                }
                
                ref_ptr<SlitPotential> sp = new SlitPotential(getStart(), getEnd());

                sp->setSlitWidth(getSlitWidth());
                if (getNumSlits() != ZERO) {
                    if (getNumSlits() == ONE) {
                        sp->addSlit(.5);
                    }
                    else {
                        Real deltaAlpha = .5 * (getSlitSeparation() + getSlitWidth()) / length();
                        sp->addSlit(.5 - deltaAlpha);
                        sp->addSlit(.5 + deltaAlpha);
                    }
                }
                return sp;
            }            
        }
    }
}