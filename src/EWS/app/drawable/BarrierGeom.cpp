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

#include <osgManipulator/Translate2DDragger>

#include "BarrierGeom.h"
#include "BarrierDragger.h"
#include "PotentialUpdater.h"
#include "EWSDebug.h"


namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            using namespace osgManipulator;
            
            
            const Real VISIBLE_BARRIER_HEIGHT = 10;
            const float VISIBLE_BARRIER_WIDTH = 6.f;
            const float BARRIER_OPACITY = .5f;
            const Vec4 BARRIER_COLOR(1.f, 0.f, 0.f, BARRIER_OPACITY);
            const Plane BARRIER_PLANE(Vec3(0, 0, 1), 0);
            
            const Vec3 BARRIER_START_ALPHA(0, 0, 0);
            const Vec3 BARRIER_END_ALPHA(1, 0, 0);
            
            /** Primary constructor. */
            BarrierGeom::BarrierGeom(Barrier& dataModel) 
            : DrawableQtAdapter(&dataModel), _dataModel(dataModel), _switch(new Switch),
            _barrierGeom(new PositionAttitudeTransform), _startKnob(new Knob), _endKnob(new Knob) {
                
                setColor(BARRIER_COLOR); 
                
                _switch->setNewChildDefaultValue(true);
                addChild(_switch.get());
                _switch->addChild(_barrierGeom.get());
                
                _switch->addChild(_startKnob.get());
                _switch->addChild(_endKnob.get());
                
                updateGeom();
                
                // Callback to detect when we've been moved
                // and update the databmodel.
                setUpdateCallback(new PotentialUpdater);
                
                respondToSignals(true);
            }
            
            
            BarrierGeom::~BarrierGeom() {
                removeChild(static_cast<unsigned int>(0), 1);
            }
            
            void BarrierGeom::respondToSignals(bool respond) {
                if(respond) {
                    connect(&_dataModel, SIGNAL(dataChanged()), SLOT(updateGeom()), Qt::UniqueConnection);
                }
                else {
                    _dataModel.disconnect(this);
                }
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
                if(enabled) {
                    _switch->setAllChildrenOn();
                }
                else {
                    _switch->setAllChildrenOff();
                }
            }
            
            void BarrierGeom::updateGeom() {
                _barrierGeom->removeChildren(0, _barrierGeom->getNumChildren());
                const Vec2& start = _dataModel.getStart();
                _barrierGeom->setPosition(Vec3d(start.x(), start.y(), 0.f));
                const Vec2& end = _dataModel.getEnd();
                const Vec2 dir = (end - start);
                const Real barrierLength = _dataModel.length();
                _barrierGeom->setScale(Vec3f(barrierLength, 1.f, 1.f));
                const Real angle = atan2(dir.y(), dir.x());
                _barrierGeom->setAttitude(Quat(angle, Vec3f(0.f, 0.f, 1.f)));
                
                // Create geometric representation
                ref_ptr<Geode> geom = new Geode;
                _barrierGeom->addChild(geom);
                if (_dataModel.getNumSlits() == Barrier::ZERO) {
                    addBox(geom, 0.5f, 1.f);
                }
                else {
                    const Real slitAlpha = _dataModel.getSlitWidth() / barrierLength;
                    if (_dataModel.getNumSlits() == Barrier::ONE) {
                        Real boxLength = 0.5f - (slitAlpha / 2);
                        addBox(geom, boxLength / 2, boxLength);
                        addBox(geom, 1 - boxLength / 2, boxLength);
                    }
                    else {
                        // Assumes that barrierLength > slitSeparation + 2 * slitWidth
                        const Real separationAlpha = _dataModel.getSlitSeparation() / barrierLength;
                        // Box length for boxes at either end of barrier
                        Real boxLength = 0.5f - slitAlpha - (separationAlpha / 2);
                        addBox(geom, boxLength / 2, boxLength);
                        addBox(geom, 1 - boxLength / 2, boxLength);
                        // Box between the two slits
                        addBox(geom, 0.5f, separationAlpha);
                    }
                }
                updateKnobs();
                
                setEnabled(_dataModel.isEnabled());
            }
            
            void BarrierGeom::updateKnobs() {
                _startKnob->setPosition(Vec3(_dataModel.getStart(), 0));
                _endKnob->setPosition(Vec3(_dataModel.getEnd(), 0));
            }
        }
    }
}
