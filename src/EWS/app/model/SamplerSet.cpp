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

#include "SamplerSet.h"
#include "PointSampler.h"
#include "SimulationState.h"

namespace ews {
    namespace app {
        namespace model {
            
        
            SamplerSet::SamplerSet(SimulationState* parent)  
            : QObject(parent), _samplers() {
            }
            
            SamplerSet::~SamplerSet() {
                removeAllSamplers();


            }
            
            PointSampler* SamplerSet::createSampler() {  
                WaveModel& model = getSimulationState()->getWaveMedium().getWaveModel();
                PointSampler* retval = new PointSampler(model.getLattice(), this);
                retval->setPosition(Vec2(20, 100));
                _samplers.push_back(retval);
                emit samplerAdded(_samplers.size() - 1, retval);
                return retval;
            }

            void SamplerSet::removeAllSamplers() {
                for (QList<PointSampler*>::iterator i = _samplers.begin(); i != _samplers.end(); i++) {
                    PointSampler* s = *i;
                    removeSampler(s);
                }
            }

            void SamplerSet::removeSampler(PointSampler* s) {
                const int pos = indexOf(s);
                if(pos >= 0) {
                    const bool did = _samplers.removeOne(s);
                    if (did) {
                        s->disconnect(this);
                        emit samplerRemoved(pos, s);
                        delete s;
                    }
                }
            }
            
            
            void SamplerSet::setPaused(bool state) {
                for (PointSamplerIterator pi = _samplers.begin(); pi != _samplers.end(); pi++) {
                    (*pi)->setPaused(state);
                }
            }
            
            void SamplerSet::reset() {
                for (PointSamplerIterator pi = _samplers.begin(); pi != _samplers.end(); pi++) {
                    (*pi)->reset();
                }
                
                removeAllSamplers();
            }
            
            SimulationState* SamplerSet::getSimulationState() const {
                QObject* obj = parent();
                return qobject_cast<SimulationState*>(obj);
            }
        }
    }
}


