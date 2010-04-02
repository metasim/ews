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


#include "PointSampler.h"
#include "SimulationState.h"
#include "EWSDebug.h"

namespace ews {
    namespace app {
        namespace model {
            
            void PointSampler::sample() {
                using osg::Vec2;
                if(isEnabled() && !isPaused()) {
                    Vec2 pos = getPosition();
                    ews::physics::LatticeVal val = _lattice.getValue((int) pos.x(), (int) pos.y());
                    _history.addValue(val);
                    emit sampleHistoryChanged();
                }
            }
            
            SimulationState* PointSampler::getSimulationState() const {
                QObject* obj = parent();
                return qobject_cast<SimulationState*>(obj);
            }
        }
    }
}
