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

#ifndef SimulationState_H
#define SimulationState_H

#include <QtCore>

#include "DripSource.h"
#include "WaveMedium.h"
#include "BarrierSet.h"

namespace ews {
    namespace app {
        namespace model {
            
            /** This class is the root of the simulation state; all transient data
             *  should be available from the class and or its children.
             */
            // cond/endcond is used to tell Doxygen to ignore what's in between
            class SimulationState : /** @cond */ public QObject /** @endcond */ {
                Q_OBJECT
                
            public:
                explicit SimulationState(QObject * parent = 0);
                virtual ~SimulationState();
               
                /**
                 * Get the first drip source.
                 */
                DripSource& getDripSource1() {
                    return _dripSource1;
                }
                
                /**
                 * Get the second drip source. 
                 */
                DripSource& getDripSource2() {
                    return _dripSource2;
                }
                
                /**
                 * Get the set of defined barriers.
                 */
                BarrierSet& getBarriers() {
                    return _barriers;
                }
                
                /**
                 * Get the wave medium.
                 */
                WaveMedium& getWaveMedium() {
                    return _waveMedium;
                }
                
                /**
                 * Determine if the simulation is currently in the "paused" state.
                 */
                bool isPaused() const {
                    return _waveMedium.isPaused();
                }
                
                
            signals:
                /** 
                 * Signal fired when a wall or barrier is added to the scene.
                 */
                void objectAdded(QObject& added);
                /**
                 * Signal fired when a wall or barrier is removed from the scene.
                 */
                void objectRemoved(QObject& removed);
                
                
            public:
                /**
                 * This is a convenience method for the setup phase when
                 * signals are getting registered and we want to 
                 * associate geometry with the default simulation state
                 * objects. It should only be called by once by the setup code.
                 */
                void emitSignalsForDefaults() {
                    emit objectAdded(getWaveMedium());
                    emit objectAdded(getDripSource1());
                    emit objectAdded(getDripSource2());
                }
                
            public slots:
                /** Pause the simulation, whereby the wave propagation is stopped. */
                void setPaused(bool);
                /** Reset the simulation and view to the default view. */
                void reset();
                
                
                void forwardBarrierSetAddition(int index, Barrier* barrier);
                void forwardBarrierSetRemoval(int index, Barrier* barrier);

            private:
                Q_DISABLE_COPY(SimulationState)
                WaveMedium _waveMedium;
                DripSource _dripSource1;
                DripSource _dripSource2;
                BarrierSet _barriers;
            };
        }
    }
}
#endif // SimulationState_H
