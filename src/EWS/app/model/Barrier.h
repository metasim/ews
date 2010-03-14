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
#include <osg/Vec2>
#include "SlitPotential.h"
#include <osg/ref_ptr>
using osg::ref_ptr;
#include "EWSDefine.h"

namespace ews {
    namespace app {
        namespace model {
            using osg::Vec2;
            
            class BarrierSet;
            using ews::physics::Potential;
            
            class Barrier : public QObject {
                Q_OBJECT
                Q_ENUMS(NumSlits)
                Q_PROPERTY(Vec2 start READ getStart WRITE setStart)
                Q_PROPERTY(Vec2 end READ getEnd WRITE setEnd)
                Q_PROPERTY(NumSlits numSlits READ getNumSlits WRITE setNumSlits)
                Q_PROPERTY(Uint slitWidth READ getSlitWidth WRITE setSlitWidth)
                Q_PROPERTY(Uint slitSeparation READ getSlitSeparation WRITE setSlitSeparation)
                
            public:
                enum NumSlits { ZERO, ONE, TWO };
                
                explicit Barrier(BarrierSet* parent = 0); 
                
                virtual ~Barrier();

                
                /** Get the owner of this. */
                BarrierSet* getBarrierSet();

                
                bool isEnabled() const {
                    return _enabled;
                }
                
                NumSlits getNumSlits() const {
                    return _numSlits;
                }
                
                Uint getSlitWidth() const {
                    return _slitWidth;
                }
                
                Uint getSlitSeparation() const {
                    return _slitSeparation;
                }
                
                Real length() const {
                    return (_end - _start).length();
                }
                
                Real width() const {
                    return ews::physics::DEFAULT_WALL_THICKNESS;
                }
                
                const osg::Vec2& getStart() const {
                    return _start;
                }
                
                const osg::Vec2& getEnd() const {
                    return _end;
                }
                
            public slots:
                
                void setEnabled(bool enabled) {
                    _enabled = enabled;
                    emit dataChanged();
                }
                
                void setNumSlits(NumSlits num) {
                    _numSlits = num;
                    emit dataChanged();
                }
                
                void setSlitWidth(Uint slitWidth) {
                    if (validSettings(slitWidth, _slitSeparation)) {
                        _slitWidth = slitWidth;
                        emit dataChanged();
                    }
                }
                
                void setSlitSeparation(Uint slitSeparation) {
                    if (validSettings(_slitWidth, slitSeparation)) {
                        _slitSeparation = slitSeparation;
                        emit dataChanged();
                    }
                }
                
                void setStart(const osg::Vec2& start) {
                    _start = start;
                }
                
                void setEnd(const osg::Vec2& end) {
                    _end = end;
                }

                ref_ptr<Potential> generatePotential();

            signals:
                void dataChanged();
                
            private:
                Q_DISABLE_COPY(Barrier)
                bool validSettings(Uint slitWidth, Uint slitSeparation) {
                    return slitWidth > 0 && slitSeparation > 0 && length() > slitSeparation + 2 * slitWidth;
                }
                bool _enabled;
                NumSlits _numSlits;
                Uint _slitWidth;
                Uint _slitSeparation;
                osg::Vec2 _start;
                osg::Vec2 _end;
                Potential* _potential;
            };
        }
    }
}


#endif // __BARRIER_H

