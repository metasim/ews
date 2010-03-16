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

#ifndef __BARRIER_DRAGGER_H
#define __BARRIER_DRAGGER_H

#include <osgManipulator/Dragger>
#include <osgManipulator/Translate2DDragger>
#include <osg/Plane>

namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            using namespace osgManipulator;
            
            class BarrierDragger : public CompositeDragger {
            public:
                BarrierDragger() : CompositeDragger() {
                    
                    ref_ptr<Translate2DDragger> dragger1 = new Translate2DDragger(BARRIER_PLANE);
//                    dragger1->setMatrix(Matrix::translate(BARRIER_START_ALPHA));
                    dragger1->setColor(Vec4(1, 0, 0, 1));
                    addDragger(dragger1.get());
                    addChild(dragger1.get());
                    dragger1->setupDefaultGeometry();
                    
//                    ref_ptr<Translate2DDragger> dragger2 = new Translate2DDragger(BARRIER_PLANE);
//                    dragger2->setMatrix(Matrix::translate(BARRIER_END_ALPHA));
//                    dragger2->setColor(Vec4(0, 0, 1, 1));
//                    addDragger(dragger2.get());
//                    addChild(dragger2.get());
//                    dragger2->setupDefaultGeometry();
                    
                }
            };
            
        }
    }
}


#endif // __BARRIER_DRAGGER_H
