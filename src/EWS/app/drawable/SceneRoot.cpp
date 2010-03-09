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

#include "SceneRoot.h"
#include "DrawableFactory.h"
#include <osg/Light>
#include <osg/LightSource>


namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            SceneRoot::SceneRoot(QObject* parent) : QObject(parent), osg::PositionAttitudeTransform(), _drawables() {
                // Set up some basic lighting.
                
                osg::StateSet* state = getOrCreateStateSet(); 
                state->setMode( GL_LIGHTING, osg::StateAttribute::ON ); 
                
                state->setMode( GL_LIGHT0, osg::StateAttribute::ON );
                
                ref_ptr<Light> light = new Light; 
                light->setLightNum(1);
                light->setPosition(Vec4(10.0f, 10.0f, 100.0f, 1.0f)); 
                light->setAmbient(Vec4(0.1f, 0.1f, 0.1f, 1.0f)); 
                light->setDiffuse(Vec4(0.5f, 0.5f, 0.5f, 1.0f));
                light->setSpecular(Vec4(0.8f, 0.8f, 0.8f, 1.0f)); 
//                light->setDirection(Vec3(0.0f, 0.0f, 0.0f));
//                light->setSpotCutoff(25.f );
                
                ref_ptr<LightSource> lSource = new LightSource;
                lSource->setLight(light.get());
                lSource->setReferenceFrame(LightSource::ABSOLUTE_RF );
                addChild(lSource.get());
            }
            
            
            SceneRoot::~SceneRoot() {
            }
            
            void SceneRoot::centerScene() {
                setPosition(Vec3(0, 0, 0));
                const BoundingSphere& bounds = getBound();
                Vec3d center = bounds.center();
                center.z() = 0;
                setPosition(-center);
            }
            
            void SceneRoot::addDrawableFor(QObject& data) {
                osg::Node* geom = DrawableFactory::instance().createDrawableFor(data);
                if(geom) {
                    addChild(geom);
                    _drawables.insert(&data, geom);
                }
                
                // HACK: Process of centering scene at origin needs to be 
                // better handled outside of here
                if(_drawables.size() == 1) {
                    centerScene();
                }
            }
            
            void SceneRoot::removeDrawableFor(QObject& data) {
                osg::Node* geom = _drawables.take(&data);
                if(geom) {
                    removeChild(geom);
                }
            }
        }
    }
}
