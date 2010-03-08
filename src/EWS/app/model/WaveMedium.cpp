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


#include "WaveMedium.h"
#include "SlitPotential.h"
#include "PrecomputedPotential.h"
#include <osg/Vec2>

namespace ews {
    namespace app {
        namespace model {

            void WaveMedium::updateWaveModel() {
                using ews::physics::WaveModel;
                using ews::physics::SlitPotential;
                using ews::physics::Potential;
                using ews::physics::PrecomputedPotential;
                
                if(_waveModel) {
                    delete _waveModel;
                    _waveModel = 0;
                }
                
                _waveModel = new WaveModel(_width * _latticeDivisionsPerCentimeter, 
                                           _length * _latticeDivisionsPerCentimeter);
                
                SlitPotential* sp = new SlitPotential(osg::Vec2(10, 50), osg::Vec2(50, 10), 2);
                sp->setSlitWidth(5);
                counted_ptr<const Potential> p(sp);
                PrecomputedPotential* prePot = new PrecomputedPotential(p, _waveModel->getWidth(),
                                                                        _waveModel->getLength());
                counted_ptr<const Potential> ptrPrePot(prePot);
                _waveModel->setPotential(ptrPrePot);
            }
            
        }
    }
}