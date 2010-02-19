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

class SimulationState : public QObject
{
	Q_OBJECT
    
public:
    SimulationState(QObject * parent = 0);
    virtual ~SimulationState();
    
    DripSource* dripSource1() {
        return _dripSource1;
    }
    
    DripSource* dripSource2() {
        return _dripSource2;
    }
    
private:
    Q_DISABLE_COPY(SimulationState)
    DripSource* _dripSource1;
    DripSource* _dripSource2;
    
};

#endif // SimulationState_H
