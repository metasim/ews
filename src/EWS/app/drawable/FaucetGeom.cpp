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
#include <osg/PolygonMode>
#include <osg/MatrixTransform>
#include <osg/Material>
#include <osg/NodeCallback>
#include "FaucetGeom.h"
#include "Oscillator.h"
#include "demo/Teapot.h"
#include <QtGlobal>


namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            
            /**
             * Private class responsible for updating the oscillator simulation
             */
            class OscillatorUpdater : public NodeCallback {
                virtual void operator()(Node* node, NodeVisitor* nv) { 
                    using ews::physics::Oscillator;
                    FaucetGeom* geom = dynamic_cast<FaucetGeom*>(node);
                    if(!geom) {
                        osg::notify(osg::WARN) << "OscillatorUpdater didn't receieve the right geometry type";
                        return;
                    }
                    
                    const osg::FrameStamp* fs = nv->getFrameStamp();
                    float value = fs->getSimulationTime();
                    
                    Oscillator& o = geom->getDataModel().getOscillator();
                    o.updateTimeAndOscillator(value);
                    
                    traverse(node,nv);
                }
            };
            
            FaucetGeom::FaucetGeom(DripSource& dataModel) 
            : DrawableQtAdapter(&dataModel), _dataModel(dataModel) {
                setPosition(_dataModel.getPosition());

                setScale(Vec3(5, 5, 5));
                // Create geometric representation
                {
                    ref_ptr<osg::Geode> geode = new osg::Geode;
                    addChild(geode.get());
                
                    ref_ptr<osg::Drawable> d = new Teapot;
                    geode->addDrawable(d.get());
                }
                
                
                
                setColor(Vec4(.2f, .9f, .9f, 1.f)); 
                ref_ptr<StateSet> state = getOrCreateStateSet(); 
                state->setMode(GL_MULTISAMPLE_ARB, StateAttribute::ON); 
                
                setUpdateCallback(new OscillatorUpdater);
                
                QObject::connect(&_dataModel, SIGNAL(drip(int)), this, SLOT(drip()));
                QObject::connect(&_dataModel, SIGNAL(enabledChanged(bool)), this, SLOT(setEnabled(bool)));
                QObject::connect(&_dataModel, SIGNAL(positionChanged(osg::Vec2)), this, SLOT(setPosition(const osg::Vec2&)));
                
                setEnabled(_dataModel.isEnabled());
            }
            
            
            FaucetGeom::~FaucetGeom() {
                
            }
            
            void FaucetGeom::setColor(osg::Vec4 color) {
                ref_ptr<StateSet> state = getOrCreateStateSet(); 
                ref_ptr<Material> mat = new osg::Material; 
                mat->setDiffuse(Material::FRONT, color);
                mat->setSpecular(Material::FRONT,
                                 Vec4( 0.8f, 0.8, 0.8f, 0.8f ) ); 
                mat->setShininess(Material::FRONT, 96.f ); 
                mat->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
                state->setAttribute( mat.get() );
            }

            void FaucetGeom::setPosition(const osg::Vec2& pos) {
                PositionAttitudeTransform::setPosition(osg::Vec3d(pos.x(), pos.y(), 50));
                
//                qDebug() << "**** faucet loc: " << center.x() << center.y() << center.z();
            }
            
            
            void FaucetGeom::setEnabled(bool enabled) {
                // Need to better understand these node masks and how
                // they affect different types of node visitors
                setNodeMask(enabled ? 0xffffffff : 0);
            }
            
            void FaucetGeom::drip()  {
//                qDebug() << "drip" << __FILE__ << __LINE__;
            }
        }
    }
}
