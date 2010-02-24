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
#define  __SCENE_ROOT_H

#include <QtCore>
#include <osg/Geode>

class SceneRoot : public osg::Group
{
public:
    SceneRoot();
    virtual ~SceneRoot();
    
    
private:    
    SceneRoot(const SceneRoot& copy) {}
    void operator=(const SceneRoot& copy) {}
};


#endif // __SCENE_ROOT_H