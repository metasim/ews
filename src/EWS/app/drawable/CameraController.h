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

#ifndef __CAMERA_CONTROLLER_H
#define __CAMERA_CONTROLLER_H

#include <osgGA/TerrainManipulator>
#include <osgGA/GUIEventAdapter>

namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            
            
            
            /**
             * Specialization of OSG TerrainManipulator for
             * setting up some defaults and limiting movement to some degree.
             */
            class CameraController : public osgGA::TerrainManipulator {
            public:
                CameraController() : osgGA::TerrainManipulator() {
                    setRotationMode(osgGA::TerrainManipulator::ELEVATION_AZIM);
                    setHomePosition(Vec3d(200, 300, 200), Vec3d(0, 0, 0), Vec3d(0, 1, 0), false);
                    setMinimumDistance(10);
                }
                
                virtual ~CameraController() {}
            };
        }
    }
}


#endif //__CAMERA_CONTROLLER_H

