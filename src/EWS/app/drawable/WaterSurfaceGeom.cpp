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

#include "WaterSurfaceGeom.h"

#include <osg/Geode>
#include <osg/Shape>
#include <osg/ShapeDrawable>
namespace ews {
    namespace app {
        namespace drawable {
            WaterSurfaceGeom::WaterSurfaceGeom() : DrawableQtAdapter(NULL)
            {
                osg::Capsule* thing = new osg::Capsule;
                
                osg::ShapeDrawable* drawable = new osg::ShapeDrawable(thing);
                drawable->setColor(osg::Vec4(1, 1, 0, 1));
                
                osg::Geode* geom = new osg::Geode;
                
                geom->addDrawable(drawable);
                
//                addChild(geom);
                
            }
        }
    }
}