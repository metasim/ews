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

#include <osg/Light>
#include <osg/LightSource>
#include <osg/Node>
#include <osgManipulator/Dragger>
#include "DrawableFactory.h"
#include "PickHandler.h"

namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            using namespace osgManipulator;
            
            SceneRoot::SceneRoot(QObject* parent) 
            : QObject(parent), osg::MatrixTransform(), _drawables(), _manipCommander() {
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
                
                ref_ptr<LightSource> lSource = new LightSource;
                lSource->setLight(light.get());
                lSource->setReferenceFrame(LightSource::ABSOLUTE_RF );
                addChild(lSource.get());
                
                _manipCommander = new osgManipulator::CommandManager;
                
                addEventCallback(new PickHandler);
            }
            
            
            SceneRoot::~SceneRoot() {
            }
            
            Node* SceneRoot::setupManipulator(DrawableQtAdapter* drawable) {
                
                ref_ptr<Dragger> dragger = drawable->createDragger();
                if(!dragger) {
                    return drawable;
                }
                
                /** osg::Group node **/
                Group* group = new Group;
                
                /** Selection Node **/
                ref_ptr<Selection> selection =  new Selection;
                group->addChild(selection.get());
                selection->addChild(drawable);
                
                group->addChild(dragger);
                
                /** Starting matrix for the Dragger **/
                BoundingSphere bounds = drawable->getBound();
                float scale =  bounds.radius() * 1.5f;
                Matrixd mat = Matrix::scale(scale, scale, scale) * Matrix::translate(bounds.center());
                dragger->setMatrix(mat);
                
                /** Command Manager - connects Dragger objects with Selection objects **/
                _manipCommander->connect(*(dragger.get()), *(selection.get()));
                return group;
            }
            
            void SceneRoot::addDrawableFor(QObject& data) {
                DrawableQtAdapter* drawable = DrawableFactory::instance().createDrawableFor(data);
                if(drawable) {
                    Node* parent = setupManipulator(drawable);
                    
                    addChild(parent);
                    _drawables.insert(&data, parent);
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
