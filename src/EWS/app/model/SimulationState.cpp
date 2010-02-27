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
namespace ews {
    namespace app {
        namespace model {
            SimulationState::SimulationState(QObject * parent) :
            QObject(parent), _dripSource1(this), _dripSource2(this), 
            _waveMedium(this) {
                _dripSource1.setObjectName("dripSource1");
                _dripSource1.setEnabled(false);
                _dripSource2.setObjectName("dripSource2");
                _dripSource2.setEnabled(false);
                
                _waveMedium.setObjectName("waveMedium");
                
            }
            
            
            SimulationState::~SimulationState() 
            {
                
            }
            
        }
    }
}