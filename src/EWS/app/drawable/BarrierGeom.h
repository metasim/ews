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
#include "Barrier.h"
#include "DrawableQtAdapter.h"

namespace ews {
    namespace  app {
        namespace drawable {
            
            using ews::app::model::Barrier;
            
            class BarrierGeom : public DrawableQtAdapter {
                Q_OBJECT
            public:
                explicit BarrierGeom(Barrier& dataModel);
                virtual ~BarrierGeom();
                
                Barrier& getDataModel() {
                    return _dataModel;
                }

                void setColor(osg::Node *srcNode, const osg::Vec4& color);
                
            public slots:
                void updateData();
                
            private:
                Q_DISABLE_COPY(BarrierGeom)
                Barrier& _dataModel;
            };
        }
    }
}

#endif // __BARRIER_GEOM_H

