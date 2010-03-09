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

#include "BarrierSet.h"
#include "SimulationState.h"
#include "CompositePotential.h"
using ews::physics::CompositePotential;
#include "PrecomputedPotential.h"
using ews::physics::PrecomputedPotential;
#include <cstdlib>
using std::srand;
using std::rand;
#include <ctime>
using std::time;
#include <osg/ref_ptr>
using osg::ref_ptr;

namespace ews {
    namespace app {
        namespace model {
        
            BarrierSet::BarrierSet(SimulationState* parent)  
            : QObject(parent), _barriers(), _potentials(NULL) {
                _potentials = new CompositePotential();
                // Initialize random seed - this will probably eventually disappear
                srand(time(NULL));
            }
            
            BarrierSet::~BarrierSet() {
                if(_potentials) {
                    delete _potentials;
                }
            }
            
            
            Barrier* BarrierSet::createBarrier() {                
                Barrier* b = new Barrier(this);
                const WaveMedium& water = getSimulationState()->getWaveMedium();
                const osg::Vec2 start(rand() % water.getWidth(), rand() % water.getLength());
                const osg::Vec2 end(rand() % water.getWidth(), rand() % water.getLength());
                b->setStart(start);
                b->setEnd(end);
                const int pos = _barriers.size();
                _barriers << b;
                                
                b->setObjectName(QString("Barrier %1").arg(pos+1));
                QObject::connect(b, SIGNAL(dataChanged()), this, SLOT(updatePotentials()));
                updatePotentials();
                emit barrierAdded(pos, b);
                return b;
            }
            
            void BarrierSet::updatePotentials() {
                ref_ptr<CompositePotential> worldPot = new CompositePotential();
                for (QList<Barrier*>::iterator i = _barriers.begin(); i != _barriers.end(); i++) {
                    Barrier* b = *i;
                    worldPot->addPotential(b->generatePotential().get());
                }
                WaveModel& waveModel = getSimulationState()->getWaveMedium().getWaveModel();
                ref_ptr<PrecomputedPotential> prePot = new PrecomputedPotential(worldPot.get(), waveModel.getWidth(),
                                                                                waveModel.getLength());
                waveModel.setPotential(prePot.get());
            }

            void BarrierSet::removeBarrier(Barrier* b) {
                const int pos = indexOf(b);
                if(pos >= 0) {
                    const bool did = _barriers.removeOne(b);
                    if(did) {
                        emit barrierRemoved(pos, b);
                    }
                }
            }
            
            SimulationState* BarrierSet::getSimulationState() const {
                QObject* obj = parent();
                return qobject_cast<SimulationState*>(obj);
            }
        }
    }
}


