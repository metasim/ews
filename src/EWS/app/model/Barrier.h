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

namespace ews {
    namespace app {
        namespace model {
            using osg::Vec2;
            
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
                
                Barrier(QObject* parent = 0) 
                : QObject(parent), _enabled(true), _numSlits(ZERO), 
                _slitWidth(3), _slitSeparation(5), _start(10, 10), _end(10, 50) {
                
                }
                
                virtual ~Barrier() {}
                
                bool isEnabled() const {
                    return _enabled;
                }
                
                NumSlits getNumSlits() const {
                    return _numSlits;
                }
                
                unsigned int getSlitWidth() const {
                    return _slitWidth;
                }
                
                unsigned int getSlitSeparation() const {
                    return _slitSeparation;
                }
                
                osg::Vec2 getStart() const {
                    return _start;
                }
                
                osg::Vec2 getEnd() const {
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
                
                void setSlitWidth(unsigned int slitWidth) {
                    _slitWidth = slitWidth;
                    emit dataChanged();
                }
                
                void setSlitSeparation(unsigned int slitSeparation) {
                    _slitSeparation = slitSeparation;
                    emit dataChanged();
                }
                
                void setStart(osg::Vec2 start) {
                    _start = start;
                }
                
                void setEnd(osg::Vec2 end) {
                    _end = end;
                }


            signals:
                void dataChanged();
                
            private:
                Q_DISABLE_COPY(Barrier)
                bool _enabled;
                NumSlits _numSlits;
                unsigned int _slitWidth;
                unsigned int _slitSeparation;
                osg::Vec2 _start;
                osg::Vec2 _end;
            };
        }
    }
}


#endif // __BARRIER_H

