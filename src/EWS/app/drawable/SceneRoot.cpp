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

namespace ews {
    namespace app {
        namespace drawable {
            
            SceneRoot::SceneRoot(QObject* parent) : QObject(parent), osg::Group(), _drawables() {
            }
            
            
            SceneRoot::~SceneRoot() {
            }
            
            
            void SceneRoot::addDrawableFor(QObject& data) {
                osg::Node* geom = DrawableFactory::instance().createDrawableFor(data);
                if(geom) {
                    addChild(geom);
                    _drawables.insert(&data, geom);
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
