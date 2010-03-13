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


#ifndef __BARRIER_GEOM_H
#define __BARRIER_GEOM_H

#include <osg/Vec2>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Plane>
#include <osgManipulator/Translate2DDragger>
#include "Barrier.h"
#include "DrawableQtAdapter.h"

namespace ews {
    /**
     * Parent namespace for GUI-related code.
     */
    namespace  app {
        /**
         * Contains classes responsible for generating 3-dimensional openGL representations.
         */
        namespace drawable {
            using ews::app::model::Barrier;
            using namespace osg;
            
            /** The plane where the barriers reside. */
            extern const Plane BARRIER_PLANE;
            /** Normalized/local coordinate of barrier start position. */
            extern const Vec3 BARRIER_START_ALPHA;
            /** Normalized/local coordinate of barrier end position. */
            extern const Vec3 BARRIER_END_ALPHA;
            
            /** A 3-D geometric representation of a barrier, with or without slits. */
            class BarrierGeom : public DrawableQtAdapter {
                Q_OBJECT
            public:
                /** Standard ctor. */
                explicit BarrierGeom(Barrier& dataModel);
                
                /** Create the type of dragger this geometry type supports. */
                virtual osgManipulator::Dragger* createDragger();
                
                
                /** Get the data object this reflects. */
                Barrier& getDataModel() {
                    return _dataModel;
                }
                
                /** Set barrier color. */
                void setColor(const osg::Vec4& color);
                
                /** Set whether barrier is visible. */
                void setEnabled(bool enabled);
                
            private slots:
                void updateGeom();
                
            protected:
                /** Protected to enforce use with ref_ptr. */
                virtual ~BarrierGeom();
                
                
            private:
                Q_DISABLE_COPY(BarrierGeom)

                /** Turn on/off whether this responds to signals. */
                void respondToSignals(bool respond);
                /** Convenience method for creating a box in local coordinates. */
                void addBox(const osg::ref_ptr<osg::Geode>& geode, Real boxCenter, Real boxLength);

                Barrier& _dataModel;
                osg::ref_ptr<osg::Geode> _barrierGeom;
                osg::ref_ptr<osgManipulator::Dragger> _dragger;
                
                /** For access to respondsToSignals(bool). */
                friend class PotentialUpdater;
            };
        }
    }
}

#endif // __BARRIER_GEOM_H

