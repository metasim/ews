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
#include <osg/Material>
#include <osgManipulator/Translate2DDragger>
#include "DrawableQtAdapter.h"
#include "Knob.h"

namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            using namespace osgManipulator;
            
            class KnobDragger : public Translate2DDragger {
            public:
                KnobDragger(Knob& knob) : Translate2DDragger(Plane(osg::Z_AXIS, 0)), _knob(knob) {
                    setName("knobDragger");
                    setColor(Vec4(0.9, 0.9, 0.9, 1));
                    osgManipulator::setMaterialColor(getColor(), *this);
                    setPickColor(Vec4(1, 1, 0, 1));
                }
                virtual ~KnobDragger() {}
                
                /** Handle pick events on dragger. */
                virtual bool handle(const PointerInfo& pi, const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us) {
                    bool retval = Translate2DDragger::handle(pi, ea, us);
                    
                    switch (ea.getEventType()) {
                        case osgGA::GUIEventAdapter::PUSH: 
                        case osgGA::GUIEventAdapter::DRAG: 
                        case osgGA::GUIEventAdapter::RELEASE: 
                            _knob.setDirty(true);
                            break;
                        default:
                            break;
                    }

                    return retval;
                }

            private:
                Knob& _knob;
            };

            /** The ctor creates a mini-hierarchy with a Dragger and Selection
             * as the immediate children of this, and the selection geometry 
             * (i.e. sphere) as a child of the selection. */
            Knob::Knob(unsigned int radius) : _dirty(true) {
                setName("knob");
                // Create selection type to contain our drawable.
                _selectionNode = new Selection;
                _selectionNode->setName("knobSelection");
                _dragger = new KnobDragger(*this);

                // Put selection and dragger under same group
                addChild(_selectionNode.get());
                addChild(_dragger.get());
                
                ref_ptr<Geode> geom = new Geode;
                geom->setName("knobGeometry");
                _dragger->addChild(geom.get());
                ref_ptr<Sphere> sphere = new Sphere(Vec3(0, 0, 0), radius);
                ref_ptr<ShapeDrawable> sphereGeom = new ShapeDrawable(sphere.get());
                geom->addDrawable(sphereGeom.get());
                
//                geom->getOrCreateStateSet()->setMode(GL_LIGHTING, StateAttribute::OFF);
//                geom->setDataVariance(DYNAMIC);
                
                // Create another copy that will be invisible for the pick region. 
                sphere = new Sphere(Vec3(0, 0, 0), radius*1.25);
                sphereGeom = new ShapeDrawable(sphere.get());                
                osgManipulator::setDrawableToAlwaysCull(*sphereGeom);
                geom = new Geode;
                geom->setName("knobDraggerGeometry");
                geom->addDrawable(sphereGeom.get());
                _dragger->addChild(geom.get());
                
                
                // The command manager is requred to connect Dragger objects with Selection objects
                DrawableQtAdapter::manipCommandManager().connect(*(_dragger.get()), *(_selectionNode.get()));
            }
            
            Knob::~Knob() {
                
            }
        }
    }
}