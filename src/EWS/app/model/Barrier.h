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
                Q_PROPERTY(unsigned int slitWidth READ getSlitWidth WRITE setSlitWidth)
                Q_PROPERTY(unsigned int slitSeparation READ getSlitSeparation WRITE setSlitSeparation)
                
            public:
                enum NumSlits { ZERO, ONE, TWO };
                
                explicit Barrier(BarrierSet* parent = 0); 
                
                virtual ~Barrier();

                
                /** Get the owner of this. */
                BarrierSet* getBarrierSet();

                /**
                 * Gets enabled state of barrier. 
                 * @return true if active, false if inactive and invisible.
                 */
                bool isEnabled() const {
                    return _enabled;
                }
                
                /**
                 * Number of slits in barrer {0,1,2}
                 */
                NumSlits getNumSlits() const {
                    return _numSlits;
                }
                
                /**
                 * Size of the slit when one or more slits
                 */
                unsigned int getSlitWidth() const {
                    return _slitWidth;
                }
                
                /**
                 * Distance between slits when more than one slit.
                 */
                unsigned int getSlitSeparation() const {
                    return _slitSeparation;
                }
                
                /**
                 * Distance between start and end points. 
                 * @return |end - start|
                 */
                Real length() const {
                    return (_end - _start).length();
                }
                
                /**
                 * Fixed wall thickness.
                 */
                static Real width() {
                    return ews::physics::DEFAULT_WALL_THICKNESS;
                }
                
                /**
                 * Get the barrier start point (2-D water surface coordinates).
                 */
                const osg::Vec2& getStart() const {
                    return _start;
                }
                
                /**
                 * Get the barrier end point (2-D water surface coordinates).
                 */
                const osg::Vec2& getEnd() const {
                    return _end;
                }

                /**
                 * Get the computed potential for the current settings.
                 * It is updated when any of the parameters of this are
                 * changed. When the potential changes, the potentialChanged()
                 * signal is fired (after dataChanged() signal).
                 * @return pointer to current potential. Memory owned is by this.
                 */
                Potential* getPotential() const {
                    return _potential;
                }
                
                /**
                 * Set all the configurable parameters at once, resulting in a single
                 * dataChanged() signal and a recomputed potential.
                 * @param start start point in 2-D water suface coordinates
                 * @param end end point in 2-D water suface coordinates
                 * @param numSlits number of slits
                 * @param slitWidth size of slit(s)
                 * @param slitSeparation distance between slit edges when numSlits > ONE.
                 */
                void set(Vec2 start, Vec2 end, NumSlits numSlits, unsigned int slitWidth, unsigned int slitSeparation) {
                    _start = start;
                    _end = end;
                    _numSlits = numSlits;
                    if(_numSlits < TWO || validSettings(slitWidth, slitSeparation)) {
                        _slitWidth = slitWidth;
                        _slitSeparation = slitSeparation;
                    }
                    else {
                        unsigned int val = std::min(width(), length());
                        _slitWidth = val;
                        _slitSeparation = val;
                    }
                    
                    emit dataChanged();
                }
                
            public slots:
                
                /**
                 * Change the enabled state of this.
                 * @param enabled True to make barrier visible and active
                 * in simulation, false to hide and remove from simulation.
                 * @signals dataChanged()
                 */
                void setEnabled(bool enabled) {
                    _enabled = enabled;
                    emit dataChanged();
                }
                
                void setNumSlits(NumSlits num) {
                    _numSlits = num;
                    emit dataChanged();
                }
                
                void setSlitWidth(unsigned int slitWidth) {
                    if (validSettings(slitWidth, _slitSeparation)) {
                        _slitWidth = slitWidth;
                        emit dataChanged();
                    }
                }
                
                void setSlitSeparation(unsigned int slitSeparation) {
                    if (validSettings(_slitWidth, slitSeparation)) {
                        _slitSeparation = slitSeparation;
                        emit dataChanged();
                    }
                }
                
                void setStart(const osg::Vec2& start) {
                    _start = start;
                    emit dataChanged();
                }
                
                void setEnd(const osg::Vec2& end) {
                    _end = end;
                    emit dataChanged();
                }

            signals:
                void dataChanged();
                void potentialChanged();
            
            private slots:
                void generatePotential();
                
            private:
                Q_DISABLE_COPY(Barrier)
                bool validSettings(unsigned int slitWidth, unsigned int slitSeparation) {
                    return slitWidth > 0 && slitSeparation > 0 && length() > slitSeparation + 2 * slitWidth;
                }

                bool _enabled;
                NumSlits _numSlits;
                unsigned int _slitWidth;
                unsigned int _slitSeparation;
                osg::Vec2 _start;
                osg::Vec2 _end;
                osg::ref_ptr<Potential> _potential;
            };
        }
    }
}


#endif // __BARRIER_H

