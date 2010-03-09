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
#ifndef __SCENE_ROOT_H
#define __SCENE_ROOT_H

#include <QtCore>
#include <osg/PositionAttitudeTransform>

namespace ews {
    namespace app {
        namespace drawable {
            class SceneRoot : public QObject, public osg::PositionAttitudeTransform {
                Q_OBJECT
            public:
                explicit SceneRoot(QObject* parent = 0);
                virtual ~SceneRoot();
                
            public slots:
                void addDrawableFor(QObject& data);
                void removeDrawableFor(QObject& data);
                void centerScene();
                
            private:    
                Q_DISABLE_COPY(SceneRoot)
                
                typedef QMap<QObject*,osg::Node*> Qt2OSGMap;
                Qt2OSGMap _drawables;
            };
        }
    }
}

#endif // __SCENE_ROOT_H
