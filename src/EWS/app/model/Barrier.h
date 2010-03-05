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

#ifndef __BARRIER_H
#define __BARRIER_H


#include <QtCore>
#include <QObject>

namespace ews {
    namespace app {
        namespace model {
            class Barrier : public QObject {
                Q_OBJECT
                Q_ENUMS(NumSlits)
                Q_PROPERTY(NumSlits numSlits READ getNumSlits WRITE setNumSlits)
                
            public:
                enum NumSlits { Zero, One, Two };
                
                Barrier(QObject* parent = 0) 
                : QObject(parent), _enabled(true), _numSlits(Zero) {
                
                }
                
                virtual ~Barrier() {}
                
                NumSlits getNumSlits() const {
                    return _numSlits;
                }
                
                bool isEnabled() const {
                    return _enabled;
                }
                
            public slots:
                void setNumSlits(NumSlits num) {
                    _numSlits = num;
                    emit dataChanged();
                }
                
                void setEnabled(bool enabled) {
                    _enabled = enabled;
                    emit dataChanged();
                }
                
            signals:
                void dataChanged();
                
            private:
                Q_DISABLE_COPY(Barrier)
                bool _enabled;
                NumSlits _numSlits;

            };
        }
    }
}


#endif // __BARRIER_H

