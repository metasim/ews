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
#include "BarrierGeom.h"
#include <QtGlobal>


namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            const float VISIBLE_BARRIER_WIDTH = 6.f;
            const float VISIBLE_BARRIER_HEIGHT = 20.f;
            const float BARRIER_OPACITY = .5f;
            const Vec4 BARRIER_COLOR(1.f, 0.f, 0.f, BARRIER_OPACITY);
            
            BarrierGeom::BarrierGeom(Barrier& dataModel) 
            : DrawableQtAdapter(&dataModel), _dataModel(dataModel) {
                ref_ptr<osg::Geode> geode = new osg::Geode;
                insertChild(0, geode.get());
                updateData();
                
                setColor(BARRIER_COLOR); 
#if defined(GL_MULTISAMPLE_ARB)
                ref_ptr<StateSet> state = getOrCreateStateSet();
                state->setMode(GL_MULTISAMPLE_ARB, StateAttribute::ON);
#endif
                
                QObject::connect(&_dataModel, SIGNAL(dataChanged()), this, SLOT(updateData()));
            }
            
            
            BarrierGeom::~BarrierGeom() {
                
            }
            
            void BarrierGeom::setColor(const osg::Vec4& color) {
                ref_ptr<StateSet> state = getOrCreateStateSet(); 
                ref_ptr<Material> mat = new osg::Material; 
                mat->setDiffuse(Material::FRONT, color);
                mat->setSpecular(Material::FRONT, Vec4(0.f, 0.f, 0.f, BARRIER_OPACITY));
                mat->setAmbient(Material::FRONT, color);
                mat->setShininess(Material::FRONT, 95.f);
                state->setAttribute(mat.get());
                state->setMode(GL_BLEND, osg::StateAttribute::ON); // Activate blending (for transparency)
            }
            
            void BarrierGeom::addBox(const ref_ptr<osg::Geode>& geode, float boxCenter, float boxLength) {
                ref_ptr<osg::ShapeDrawable> d = new ShapeDrawable();
                ref_ptr<osg::Shape> s = new osg::Box(osg::Vec3(boxCenter, 0.f, 0.f),
                                                     boxLength, ews::physics::DEFAULT_WALL_THICKNESS,
                                                     VISIBLE_BARRIER_HEIGHT);
                d->setShape(s.get());
                geode->addDrawable(d.get());
            }
            
            void BarrierGeom::updateData() {
                const osg::Vec2& start = _dataModel.getStart();
                PositionAttitudeTransform::setPosition(osg::Vec3d(start.x(), start.y(), 0.f));
                const osg::Vec2& end = _dataModel.getEnd();
                const osg::Vec2 dir = (end - start);
                const float barrierLength = _dataModel.length();
                setScale(Vec3f(barrierLength, 1.f, 1.f));
                const float angle = atan2(dir.y(), dir.x());
                setAttitude(Quat(angle, Vec3f(0.f, 0.f, 1.f)));
                
                // Create geometric representation
                ref_ptr<osg::Geode> geode = dynamic_cast<osg::Geode*>(getChild(0));
                geode->removeDrawables(0, geode->getNumDrawables());
                ref_ptr<osg::Drawable> d = dynamic_cast<osg::Drawable*>(new ShapeDrawable());
                ref_ptr<osg::Shape> s = NULL;
                if (_dataModel.getNumSlits() == Barrier::ZERO) {
                    addBox(geode, 0.5f, 1.f);
                }
                else {
                    const float slitAlpha = _dataModel.getSlitWidth() / barrierLength;
                    if (_dataModel.getNumSlits() == Barrier::ONE) {
                        float boxLength = 0.5f - (slitAlpha / 2);
                        addBox(geode, boxLength / 2, boxLength);
                        addBox(geode, 1 - boxLength / 2, boxLength);
                    }
                    else {
                        // Assumes that barrierLength > slitSeparation + 2 * slitWidth
                        const float separationAlpha = _dataModel.getSlitSeparation() / barrierLength;
                        // Box length for boxes at either end of barrier
                        float boxLength = 0.5f - slitAlpha - (separationAlpha / 2);
                        addBox(geode, boxLength / 2, boxLength);
                        addBox(geode, 1 - boxLength / 2, boxLength);
                        // Box between the two slits
                        addBox(geode, 0.5f, separationAlpha);
                    }
                }
            }            
        }
    }
}
