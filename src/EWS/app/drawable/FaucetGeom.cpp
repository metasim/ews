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
#include "Teapot.h"
#include <QtGlobal>
#include "EWSDebug.h"

namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
                using ews::physics::Oscillator;
            
            /**
             * Private class responsible for updating the oscillator simulation
             */
            class FaucetUpdater : public NodeCallback {
                
            private:
                void updateOscillator(FaucetGeom* geom, NodeVisitor* nv) {
                    
                    if(!geom->getDataModel().isPaused()) {
                        const FrameStamp* fs = nv->getFrameStamp();
                        Real value = fs->getSimulationTime();
                        
                        Oscillator& o = geom->getDataModel().getOscillator();
                        o.updateTimeAndOscillator(value);
                    }
                }
                
                void detectAndHandleMove(FaucetGeom* geom, NodeVisitor* nv) {
                    const NodePath& nodePath = nv->getNodePath();
                    Matrix currMat = osg::computeLocalToWorld(nodePath);
                    if(currMat != _localToWorld) {
                        _localToWorld = currMat;
                        
                        Vec3 loc = _localToWorld.getTrans();
                        
                        qDebug() << "movement happened, new pos: " << loc;
                        
                        DripSource& dm = geom->getDataModel();
                        dm.blockSignals(true);
                        dm.setPosition(Vec2(loc.x(), loc.y()));
                        dm.blockSignals(false);
                        
                    }
                    
                }
                
                Matrix _localToWorld;
                
            public:
                
                /** Callback method. */
                virtual void operator()(Node* node, NodeVisitor* nv) { 
                    FaucetGeom* geom = dynamic_cast<FaucetGeom*>(node);
                    if(geom) {
                        detectAndHandleMove(geom, nv);
                        updateOscillator(geom, nv);
                    }
                    else {
                        notify(osg::WARN) << "OscillatorUpdater didn't receieve the right geometry type";
                        return;
                    }
                    
                    traverse(node,nv);
                }
            };
            
            FaucetGeom::FaucetGeom(DripSource& dataModel) 
            : DrawableQtAdapter(&dataModel), _dataModel(dataModel), _dragger() {
                
                _dragger = new Translate2DDragger(osg::Plane(osg::Vec3f(0, 0, 1), 0));
                _dragger->setupDefaultGeometry();
                
                setPosition(_dataModel.getPosition());
                
                createGeom();


                // Attach render-loop callback to update oscillator.
                setUpdateCallback(new FaucetUpdater);
                
                QObject::connect(&_dataModel, SIGNAL(drip(int)), this, SLOT(drip()));
                QObject::connect(&_dataModel, SIGNAL(enabledChanged(bool)), this, SLOT(setEnabled(bool)));
                QObject::connect(&_dataModel, SIGNAL(positionChanged(osg::Vec2)), this, SLOT(setPosition(const osg::Vec2&)));
                
                setEnabled(_dataModel.isEnabled());

                // Antialias faucet.
#if defined(GL_MULTISAMPLE_ARB)
                ref_ptr<StateSet> state = getOrCreateStateSet();
                state->setMode(GL_MULTISAMPLE_ARB, StateAttribute::ON);
#endif
            }
            
            
            FaucetGeom::~FaucetGeom() {
            }
            
            
            void FaucetGeom::createGeom() {
                // Create geometric representation

                ref_ptr<PositionAttitudeTransform> trans = new PositionAttitudeTransform;
                addChild(trans.get());
                
                ref_ptr<Geode> geode = new Geode;
                trans->addChild(geode.get());
                
                ref_ptr<osg::Drawable> d = new Teapot;
                geode->addDrawable(d.get());

                // Orient the teapot so it looks like the source of drips
                // is at the spout.
                trans->setAttitude(Quat(osg::DegreesToRadians(50.0f), Vec3f(0, 1, 0)));
                trans->setPosition(Vec3(-24, 0, 0));
                trans->setScale(Vec3(5, 5, 5));
                
                setColor(Vec4(.2f, .9f, .9f, 1.f)); 
            }
            
            void FaucetGeom::setColor(osg::Vec4 color) {
                ref_ptr<StateSet> state = getOrCreateStateSet(); 
                ref_ptr<Material> mat = new osg::Material; 
                mat->setDiffuse(Material::FRONT, color);
                mat->setSpecular(Material::FRONT,
                                 Vec4( 0.8f, 0.8f, 0.8f, 0.8f ) ); 
                mat->setShininess(Material::FRONT, 96.f ); 
                mat->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
                state->setAttribute( mat.get() );
            }

            void FaucetGeom::setPosition(const osg::Vec2& pos) {
                PositionAttitudeTransform::setPosition(osg::Vec3d(pos.x(), pos.y(), 50));
            }
            
            
            void FaucetGeom::setEnabled(bool enabled) {
                // Need to better understand these node masks and how
                // they affect different types of node visitors
                setNodeMask(enabled ? 0xffffffff : 0);
                // Hack... really need to change to a switch node arragement.
                _dragger->setNodeMask(enabled ? 0xffffffff : 0);
            }
            
            void FaucetGeom::drip()  {
//                qDebug() << "drip" << __FILE__ << __LINE__;
            }
        };
    }
}
