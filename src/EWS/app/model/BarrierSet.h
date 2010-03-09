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

#ifndef __BARRIERSET_H
#define __BARRIERSET_H


#include <QObject>
#include <QList>
#include "Barrier.h"
#include "CompositePotential.h"

namespace ews {
    namespace app {
        namespace model {
            using ews::physics::CompositePotential;
            
            class SimulationState;
            
            class BarrierSet : public QObject {
                Q_OBJECT
            public:
                BarrierSet(SimulationState* parent = 0);
                virtual ~BarrierSet();
                
                Barrier* createBarrier(); 
                
                SimulationState* getSimulationState() const;
                
                int size() const {
                    return _barriers.size();
                }
                
                void removeBarrier(Barrier* b);
                
                int indexOf(Barrier* b) {
                    return _barriers.indexOf(b);
                }
                
                Barrier* barrierAt(unsigned int index) const {
                    return _barriers[index];
                }
                
            signals:
                void barrierAdded(int,Barrier*);
                void barrierRemoved(int,Barrier*);
                
            private slots:
                void updatePotentials();
                
            private:
                Q_DISABLE_COPY(BarrierSet)
                QList<Barrier*> _barriers;
                CompositePotential* _potentials;
                
            };
        }
    }
}


#endif // __BARRIERSET_H
