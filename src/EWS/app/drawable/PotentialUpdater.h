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

#ifndef __POTENTIAL_UPDATER_H
#define __POTENTIAL_UPDATER_H

#include <osg/NodeCallback>
#include "BarrierGeom.h"
#include "EWSDebug.h"

namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            using namespace osgManipulator;

            
            /**
             * Class responsible for updating the potential data model when BarrierGeom changes.
             */
            class PotentialUpdater : public NodeCallback {
                
            private:
                void detectAndHandleMove(BarrierGeom* geom, NodeVisitor* nv) {
                    const NodePath& nodePath = nv->getNodePath();
                    Matrix currMat = osg::computeLocalToWorld(nodePath);
                    
                    // If the barrier has moved we need to recompute the start
                    // and end locations.
                    if(currMat != _localToWorld) {
                        _localToWorld = currMat;
                        
                        Vec3 start = BARRIER_START_ALPHA * _localToWorld;
                        Vec3 end = BARRIER_END_ALPHA * _localToWorld;
                        
                        qDebug() << "movement happened:" << start << end; 
                        
                        Barrier& dm = geom->getDataModel();
                        geom->respondToSignals(false);
                        dm.setStart(Vec2(start.x(), start.y()));
                        dm.setEnd(Vec2(end.x(), end.y()));
                        geom->respondToSignals(true);
                    }
                    
                }
                
                Matrix _localToWorld;
                
            public:
                
                /** Callback method. */
                virtual void operator()(Node* node, NodeVisitor* nv) { 
                    BarrierGeom* geom = dynamic_cast<BarrierGeom*>(node);
                    if(geom) {
                        detectAndHandleMove(geom, nv);
                    }
                    else {
                        notify(osg::WARN) << "PotentialUpdater didn't receieve the right geometry type";
                        return;
                    }
                    
                    traverse(node,nv);
                }
            };
        }
    }
}


#endif // __POTENTIAL_UPDATER_H

