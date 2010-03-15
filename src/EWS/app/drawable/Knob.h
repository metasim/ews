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
#include "EWSDebug.h"

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
                Knob(unsigned int radius = 3);

                /** Get the knob's current location in global coordinates. */
                Vec2 currXYLocation() const {
                    MatrixList transforms = _selectionNode->getWorldMatrices();
                    Matrix toWorld = transforms[0];
                    Vec3 translation = toWorld.getTrans();
                    return Vec2(translation.x(), translation.y());
                }
                void setPosition(Vec3 pos) {
                    using ews::util::debug::qstr;
                    QTRACE1(qstr(getName()) + qstr(": pos=") + qstr(pos));
                    setMatrix(Matrix::translate(pos));
                }
                
                /** Control the dirty flag. */
                void setDirty(bool state) {
                    _dirty = state;
                }
                
                /** Determine if the dirty flag has been set, which is the
                 * case after a drag operation has occurred. It is up to
                 * the client code to reset the dirty flag when appropriate. */
                bool isDirty() const {
                    return _dirty;
                }
                
                
            protected:
                virtual ~Knob();
                
                
            private:
                ref_ptr<Dragger> _dragger;
                ref_ptr<Selection> _selectionNode;
                bool _dirty;

            };
            
        }
    }
}


#endif // __BARRIER_DRAGGER_H
