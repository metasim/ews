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

namespace ews {
    namespace app {
        namespace model {
            class BarrierSet : public QObject {
                Q_OBJECT
                BarrierSet(QObject* parent = 0) {}
                virtual ~BarrierSet() {}
                
                Barrier* createBarrier() {
                    Barrier* b = new Barrier(this);
                    _barriers << b;
                    emit barrierAdded(b);
                    return b;
                }
                
                void removeBarrier(Barrier* b) {
                    _barriers.removeOne(b);
                    emit barrierRemoved(b);
                }
                
                int indexOf(Barrier* b) {
                    return _barriers.indexOf(b);
                }
                
            signals:
                void barrierAdded(Barrier*);
                void barrierRemoved(Barrier*);
                
            private:
                Q_DISABLE_COPY(BarrierSet)
                QList<Barrier*> _barriers;
            };
        }
    }
}


#endif __BARRIERSET_H