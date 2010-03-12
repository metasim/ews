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

#include <osg/Drawable>
#include <osg/ShapeDrawable>
#include <osg/PolygonMode>
#include <osg/MatrixTransform>
#include <osg/Material>
#include <osg/NodeCallback>
#include <osgManipulator/Translate2DDragger>
#include <osgManipulator/CommandManager>
#include "BarrierGeom.h"
#include <QtGlobal>


namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            const Real VISIBLE_BARRIER_HEIGHT = 10;

            using namespace osgManipulator;
            
            
            const float VISIBLE_BARRIER_WIDTH = 6.f;
            const float BARRIER_OPACITY = .5f;
            const Vec4 BARRIER_COLOR(1.f, 0.f, 0.f, BARRIER_OPACITY);
            
            BarrierGeom::BarrierGeom(Barrier& dataModel) 
            : DrawableQtAdapter(&dataModel), _dataModel(dataModel), _barrierGeom(new Geode) {
                setColor(BARRIER_COLOR); 
                
                addChild(_barrierGeom);
                updateGeom();
                
                QObject::connect(&_dataModel, SIGNAL(dataChanged()), this, SLOT(updateGeom()));
            }
            
            
            BarrierGeom::~BarrierGeom() {
                removeChild(static_cast<unsigned int>(0), 1);
            }
            
            
            void BarrierGeom::setColor(const Vec4& color) {
                ref_ptr<StateSet> state = getOrCreateStateSet(); 
                ref_ptr<Material> mat = new Material; 
                mat->setDiffuse(Material::FRONT, color);
                mat->setSpecular(Material::FRONT, Vec4(0.f, 0.f, 0.f, BARRIER_OPACITY));
                mat->setAmbient(Material::FRONT, color);
                mat->setShininess(Material::FRONT, 95.f);
                state->setAttribute(mat.get());
                state->setMode(GL_BLEND, StateAttribute::ON); // Activate blending (for transparency)
            }
            
            void BarrierGeom::addBox(const ref_ptr<Geode>& geode, Real boxCenter, Real boxLength) {
                ref_ptr<ShapeDrawable> d = new ShapeDrawable();
                const Real boxWidth = _dataModel.width();
                ref_ptr<Shape> s = new Box(Vec3(boxCenter, 0.f, 0.f),
                                                     boxLength, boxWidth,
                                                     VISIBLE_BARRIER_HEIGHT);
                d->setShape(s.get());
                geode->addDrawable(d.get());
            }
            
            void BarrierGeom::setEnabled(bool enabled) {
                setNodeMask(enabled ? 0xffffffff : 0);
            }
            
            void BarrierGeom::updateGeom() {
                const Vec2& start = _dataModel.getStart();
                PositionAttitudeTransform::setPosition(Vec3d(start.x(), start.y(), 0.f));
                const Vec2& end = _dataModel.getEnd();
                const Vec2 dir = (end - start);
                const Real barrierLength = _dataModel.length();
                setScale(Vec3f(barrierLength, 1.f, 1.f));
                const Real angle = atan2(dir.y(), dir.x());
                setAttitude(Quat(angle, Vec3f(0.f, 0.f, 1.f)));
                
                // Create geometric representation
                _barrierGeom->removeDrawables(0, _barrierGeom->getNumDrawables());
                ref_ptr<Drawable> d = dynamic_cast<Drawable*>(new ShapeDrawable());
                ref_ptr<Shape> s = NULL;
                if (_dataModel.getNumSlits() == Barrier::ZERO) {
                    addBox(_barrierGeom, 0.5f, 1.f);
                }
                else {
                    const Real slitAlpha = _dataModel.getSlitWidth() / barrierLength;
                    if (_dataModel.getNumSlits() == Barrier::ONE) {
                        Real boxLength = 0.5f - (slitAlpha / 2);
                        addBox(_barrierGeom, boxLength / 2, boxLength);
                        addBox(_barrierGeom, 1 - boxLength / 2, boxLength);
                    }
                    else {
                        // Assumes that barrierLength > slitSeparation + 2 * slitWidth
                        const Real separationAlpha = _dataModel.getSlitSeparation() / barrierLength;
                        // Box length for boxes at either end of barrier
                        Real boxLength = 0.5f - slitAlpha - (separationAlpha / 2);
                        addBox(_barrierGeom, boxLength / 2, boxLength);
                        addBox(_barrierGeom, 1 - boxLength / 2, boxLength);
                        // Box between the two slits
                        addBox(_barrierGeom, 0.5f, separationAlpha);
                    }
                }
                setEnabled(_dataModel.isEnabled());
            }            
        }
    }
}
