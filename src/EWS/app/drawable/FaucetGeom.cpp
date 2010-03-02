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
#include "FaucetGeom.h"
#include "demo/Teapot.h"


//class FaucetDrawCallback : public osg::Drawable::DrawCallback {
//    virtual void drawImplementation (osg::RenderInfo&, const osg::Drawable*) const;
//};
//void FaucetDrawCallback::drawImplementation (osg::RenderInfo& ri, const osg::Drawable* d) const
//{
//    glColor3f(0, 1, 1);
//    d->drawImplementation(ri);
//}


namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            
            FaucetGeom::FaucetGeom(DripSource& settings) : 
            DrawableQtAdapter(&settings), _settings(settings){
                // Move somewhere off origin.
                Matrixf m;
                m.makeScale(5, 5, 5);
                m.preMultRotate(Quat(M_PI/4.0f, Vec3f(0, 1, 0)));
                m.postMultTranslate(Vec3f(-25, 0, 50));
                setMatrix(m);
                
                // Create geometric representation
                {
                    ref_ptr<osg::Geode> geode = new osg::Geode;
                    addChild(geode.get());
                
                    ref_ptr<osg::Drawable> d = new Teapot;
//                d->setDrawCallback(new FaucetDrawCallback);
                    geode->addDrawable(d.get());
                }
                
                
                {
                    // Define material
                    StateSet* state = getOrCreateStateSet(); 
                    ref_ptr<Material> mat = new osg::Material; 
                    mat->setDiffuse(Material::FRONT,
                                    Vec4( .2f, .9f, .9f, 1.f ) ); 
                    mat->setSpecular(Material::FRONT,
                                     Vec4( 1.f, 1.f, 1.f, 1.f ) ); 
                    mat->setShininess(Material::FRONT, 96.f ); 
                    mat->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
                    state->setAttribute( mat.get() );
                    
                    state->setMode(GL_MULTISAMPLE_ARB,
                                                         StateAttribute::ON); 
                }
                
                QObject::connect(&_settings, SIGNAL(drip(int)), this, SLOT(drip(int)));
                QObject::connect(&_settings, SIGNAL(enabledChanged(bool)), this, SLOT(setEnabled(bool)));

                
                setEnabled(_settings.enabled());
            }
            
            
            FaucetGeom::~FaucetGeom() {
                
            }
            
            
            void FaucetGeom::setEnabled(bool enabled) {
                // Need to better understand these node masks and how
                // they affect different types of node visitors
                setNodeMask(enabled ? 0xffffffff : 0);
            }
            
            void FaucetGeom::drip(int amplitude)  {
                static bool prev = false;
                ref_ptr<StateSet> state = getOrCreateStateSet();
                state->setDataVariance(Object::DYNAMIC);
                // Create a PolygonMode attribute 
                ref_ptr<PolygonMode> pm = new PolygonMode(PolygonMode::FRONT_AND_BACK, PolygonMode::LINE ); 
                pm->setDataVariance(Object::DYNAMIC);
                // Force wireframe rendering. 
                prev = !prev;
                StateAttribute::Values on = prev ? StateAttribute::ON : StateAttribute::OFF;
//                state->setAttributeAndModes(pm.get(), on | StateAttribute::OVERRIDE); 
            }
        }
    }
}
