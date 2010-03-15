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

#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/Plane>
#include <osgManipulator/AntiSquish>
#include "DrawableQtAdapter.h"
#include "Knob.h"

namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            using namespace osgManipulator;

            /** The ctor creates a mini-hierarchy with a Dragger and Selection
             * as the immediate children of this, and the selection geometry 
             * (i.e. sphere) as a child of the selection. */
            Knob::Knob() {
                // Create selection type to contain our drawable.
                _selectionNode =  new Selection;
                _dragger = new Translate2DDragger(Plane(Vec3(0, 0, 1), 0));
                
                // Put selection and dragger under same group
                addChild(_selectionNode.get());
                addChild(_dragger.get());
                
                ref_ptr<Geode> geom = new Geode;
                _selectionNode->addChild(geom.get());
                ref_ptr<ShapeDrawable> sphere = new ShapeDrawable(new Sphere(Vec3(0, 0, 0), 10));
                sphere->setColor(Vec4(1, 1, 0, 1));
                geom->addDrawable(sphere.get());
                geom->getOrCreateStateSet()->setMode(GL_LIGHTING, StateAttribute::OFF);
                
                // The command manager is requred to connect Dragger objects with Selection objects
                DrawableQtAdapter::manipCommandManager().connect(*(_dragger.get()), *(_selectionNode.get()));
            }
            
            Knob::~Knob() {
                
            }
        }
    }
}