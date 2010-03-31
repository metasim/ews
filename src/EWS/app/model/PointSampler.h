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

#ifndef __POINT_SAMPLER_H
#define __POINT_SAMPLER_H

#include <QtCore>
#include <osg/Vec2>
#include "Lattice.h"
#include "EWSDefine.h"
#include <osg/BoundsChecking>

namespace ews {
    namespace app {
        namespace model {
            using ews::physics::Lattice;
            using ews::Uint;
            
            class SimulationState;
            
            /** This class is responsible for sampling the lattice when its
             * \c sample() method is called, and maintaining a history of
             * values up to a specified history length.
             */
            class PointSampler : public QObject {
                Q_OBJECT
                
            public:
                /**
                 * Standard ctor.
                 * @param lattice The lattice from which samples should be taken.
                 * @param size the total size of the history buffer.
                 * @param parent Qt parent. 
                 */
                PointSampler(const Lattice& lattice, Uint size = 1024, QObject * parent = 0) 
                : QObject(parent), _position(0, 0), _enabled(false), _lattice(lattice) {}
                
                virtual ~PointSampler() {}
                
                /**
                 * Get location in lattice. 
                 */
                osg::Vec2 getPosition() const {
                    return _position;
                }
                
                /**
                 * Determine if sampler is active.
                 */
                bool isEnabled() const {
                    return _enabled;
                }
                
                /**
                 * Gets the SimulationState parent associated with this.
                 * @return Parent SimulationState.
                 */
                SimulationState* getSimulationState() const;
                
            public slots:
                /** Set the location in the lattice where sampling should occur. */
                void setPosition(osg::Vec2 pos) {
                    // Clamp to have values inside lattice.
                    osg::clampArrayElementBetweenRange<osg::Vec2, Uint>(pos, 0, 0, (Uint)_lattice.getWidth() - 1, "sampler x position");
                    osg::clampArrayElementBetweenRange<osg::Vec2, Uint>(pos, 1, 0, (Uint)_lattice.getLength() - 1, "sampler y position");
                    _position = pos;
                    emit positionChanged(_position);
                }

                /** Change enabled state of sampling. */
                void setEnabled(bool state) {
                    _enabled = state;
                    emit enabledChanged(state);
                }
                
                /** Perform a lattice sample operation and store in history. */
                void sample();
                
            signals:
                /** Fired when the location changes. */
                void positionChanged(osg::Vec2);
                /** Fired when enabled state changes. */
                void enabledChanged(bool);
                
            private:
                Q_DISABLE_COPY(PointSampler);
                
                osg::Vec2 _position;
                bool _enabled;
                const Lattice& _lattice;
            };
        }
    }
}

#endif // __POINT_SAMPLER_H
