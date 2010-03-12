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
            SceneRoot::SceneRoot(QObject* parent) 
            : QObject(parent), osg::PositionAttitudeTransform(), _drawables(), _manipCommander() {
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
                
                
                _manipCommander = new osgManipulator::CommandManager;
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
            
            osg::Node* SceneRoot::setupManipulator(DrawableQtAdapter* drawable) {
                
                Dragger* dragger = drawable->createDragger();
                if(!dragger) {
                    return drawable;
                }
                
                /** The Geometry for the TabBoxDragger  **/
                osg::ref_ptr<osg::Geode> geom2 = new osg::Geode();
                osg::ref_ptr<osg::ShapeDrawable> shape = 
                new osg::ShapeDrawable(new osg::Cone(osg::Vec3(-100.0,0.0,30.0), 20.0f, 50.0f));
                shape->setColor(osg::Vec4(0.2f, 0.6f, 0.2f, 1.0f));
                geom2->addDrawable(shape.get());
                
                /** osg::Group node **/
                group = new osg::Group();
                arTransform->addChild(group.get());    
                
                /** Selection Node **/
                selection =  new osgManipulator::Selection();
                group->addChild(selection.get());
                selection->addChild(geom2);
                
                /** Dragger Node:
                 * This one is a TabBoxDragger for both scaling and translation **/
                dragger = new osgManipulator::TabBoxDragger();
                dragger->setupDefaultGeometry();
                group->addChild(dragger.get());
                
                /** Starting matrix for the Dragger **/
                scale = geom2->getBound().radius() * 1.5f;
                mat = osg::Matrix::scale(scale, scale, scale) * osg::Matrix::translate(geom2->getBound().center());
                dragger->setMatrix(mat);
                
                /** Command Manager - connects Dragger objects with Selection objects **/
                commandManager->connect(*(dragger.get()), *(selection.get()));                
                
                
                
            }
            
            void SceneRoot::addDrawableFor(QObject& data) {
                DrawableQtAdapter* drawable = DrawableFactory::instance().createDrawableFor(data);
                if(drawable) {
                    osg::Node* parent = setupManipulator(drawable);
                    
                    addChild(parent);
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
                qDebug() << "removing a drawable node";
                if(geom) {
                    removeChild(geom);
                }
            }
        }
    }
}
