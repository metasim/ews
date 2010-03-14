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

#ifndef __DRAWABLE_ADAPTER_H
#define __DRAWABLE_ADAPTER_H

#include <QtCore>
#include <osg/PositionAttitudeTransform>
#include <osgManipulator/Dragger>

namespace ews {
    namespace app {
        namespace drawable {
            /**
             *  Abstract base class for all geometry, serving as an adapter between the Qt and OSG worlds.
             */
            // cond/endcond is used to tell Doxygen to ignore what's in between
            class DrawableQtAdapter : /** @cond */ public QObject, public osg::PositionAttitudeTransform /** @endcond */ {
                Q_OBJECT
                
            public:
                /**
                 * Standard ctor.
                 */
                explicit DrawableQtAdapter(QObject* peer);
                
                /** If the geometry supports user interaction, the subclass
                 *  should return a dragger instance that will be registered
                 *  with the SceneRoot. Reiver takes responsibility for 
                 *  instance memory.
                 * @return Dragger to register, or NULL for none.
                 */
                virtual osgManipulator::Dragger* createDragger() = 0;
                
            protected:
                /** Protected to encourage use with ref_ptr. */
                virtual ~DrawableQtAdapter();
                
                
            private:
                Q_DISABLE_COPY(DrawableQtAdapter)
            };
            
            inline QDebug operator<<(QDebug dbg, const DrawableQtAdapter &ds) {
                dbg << ds.metaObject()->className() << "{";
                dbg << "name=" << ds.objectName() << ",";
                dbg << "}";
                return dbg;
            }
        }
    }
    
}

#endif // __DRAWABLE_ADAPTER_H


