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

#ifndef __KNOB_H
#define __KNOB_H

#include <osg/MatrixTransform>
#include <osgManipulator/Dragger>

namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            using namespace osgManipulator;
            
            /** The ctor creates a mini-hierarchy with a Dragger and Selection
             * as the immediate children of this, and the selection geometry 
             * (i.e. sphere) as a child of the selection. */
            class Knob : public MatrixTransform {
            public:
                Knob();
                
                Vec2d currLocation() const;
                
                void setPosition(Vec3 pos) {
                    setMatrix(Matrix::translate(pos));
                }
                
                
            protected:
                virtual ~Knob();
                
                
            private:
                ref_ptr<Dragger> _dragger;
                ref_ptr<Selection> _selectionNode;
                
            };
            
        }
    }
}


#endif // __BARRIER_DRAGGER_H
