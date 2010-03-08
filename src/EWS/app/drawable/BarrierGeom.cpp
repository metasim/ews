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

#include <osg/Geode>
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
            
            BarrierGeom::BarrierGeom(Barrier& dataModel) 
            : DrawableQtAdapter(&dataModel), _dataModel(dataModel) {
                ref_ptr<osg::Geode> geode = new osg::Geode;
                insertChild(0, geode.get());
                updateData();
                
                setColor(Vec4(.9f, .2f, .2f, .5f)); 
                ref_ptr<StateSet> state = getOrCreateStateSet();
#if defined(GL_MULTISAMPLE_ARB)
                state->setMode(GL_MULTISAMPLE_ARB, StateAttribute::ON);
#endif
                
                QObject::connect(&_dataModel, SIGNAL(dataChanged()), this, SLOT(updateData()));
            }
            
            
            BarrierGeom::~BarrierGeom() {
                
            }
            
            void BarrierGeom::setColor(osg::Vec4 color) {
                ref_ptr<StateSet> state = getOrCreateStateSet(); 
                ref_ptr<Material> mat = new osg::Material; 
                mat->setDiffuse(Material::FRONT, color);
                mat->setSpecular(Material::FRONT,
                                 Vec4( 0.8f, 0.8f, 0.8f, 0.8f )); 
                mat->setShininess(Material::FRONT, 96.f ); 
                mat->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
                state->setAttribute(mat.get());
            }
            
            void BarrierGeom::updateData() {
                const osg::Vec2& start = _dataModel.getStart();
                PositionAttitudeTransform::setPosition(osg::Vec3d(start.x(), start.y(), 0.f));
                const osg::Vec2& end = _dataModel.getEnd();
                const osg::Vec2 dir = (end - start);
                const float barrierLength = dir.length();
                setScale(Vec3f(barrierLength, 1.f, 1.f));
                const float angle = atan2(dir.y(), dir.x());
                setAttitude(Quat(angle, Vec3f(0.f, 0.f, 1.f)));
                
                // Create geometric representation
                ref_ptr<osg::Geode> geode = dynamic_cast<osg::Geode*>(getChild(0));
                geode->removeDrawables(0, geode->getNumDrawables());
                ref_ptr<osg::Drawable> d = dynamic_cast<osg::Drawable*>(new ShapeDrawable());
                ref_ptr<osg::Shape> s = NULL;
                if (_dataModel.getNumSlits() == Barrier::ZERO) {
                    osg::Vec3 center(0.5f, 0.f, 0.f);
                    s = new osg::Box(center, 1.f, VISIBLE_BARRIER_WIDTH, VISIBLE_BARRIER_HEIGHT);
                    d->setShape(s.get());
                    geode->addDrawable(d.get());
                }
                else {
                    const float slitAlpha = _dataModel.getSlitWidth() / barrierLength;
                    if (_dataModel.getNumSlits() == Barrier::ONE) {
                        float boxLength = 0.5f - (slitAlpha / 2);
                        s = new osg::Box(osg::Vec3(boxLength / 2, 0.f, 0.f),
                                         boxLength, VISIBLE_BARRIER_WIDTH, VISIBLE_BARRIER_HEIGHT);
                        d->setShape(s.get());
                        geode->addDrawable(d.get());
                        s = new osg::Box(osg::Vec3(1 - (boxLength / 2), 0.f, 0.f),
                                         boxLength, VISIBLE_BARRIER_WIDTH, VISIBLE_BARRIER_HEIGHT);
                        d = new ShapeDrawable();
                        d->setShape(s.get());
                        geode->addDrawable(d.get());
                    }
                    else {
                        // Assumes that barrierLength > slitSeparation + 2 * slitWidth
                        // TODO Make sure this is enforced
                        const float separationAlpha = _dataModel.getSlitSeparation() / barrierLength;
                        // Box length for boxes at either end of barrier
                        float boxLength = 0.5f - slitAlpha - (separationAlpha / 2);
                        s = new osg::Box(osg::Vec3(boxLength / 2, 0.f, 0.f),
                                         boxLength, VISIBLE_BARRIER_WIDTH, VISIBLE_BARRIER_HEIGHT);
                        d->setShape(s.get());
                        geode->addDrawable(d.get());
                        s = new osg::Box(osg::Vec3(1 - (boxLength / 2), 0.f, 0.f),
                                         boxLength, VISIBLE_BARRIER_WIDTH, VISIBLE_BARRIER_HEIGHT);
                        d = new ShapeDrawable();
                        d->setShape(s.get());
                        // Box between the two slits
                        geode->addDrawable(d.get());                        
                        s = new osg::Box(osg::Vec3(0.5f, 0.f, 0.f),
                                         separationAlpha, VISIBLE_BARRIER_WIDTH, VISIBLE_BARRIER_HEIGHT);
                        d = new ShapeDrawable();
                        d->setShape(s.get());
                        geode->addDrawable(d.get());                        
                    }
                }
            }            
        }
    }
}
