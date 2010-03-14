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



#ifndef __FAUCET_GEOM_H
#define __FAUCET_GEOM_H

#include <osg/Vec2>
#include <osg/Group>
#include <osgManipulator/Translate2DDragger>
#include "DripSource.h"
#include "DrawableQtAdapter.h"

namespace ews {
    namespace  app {
        namespace drawable {
            
            using ews::app::model::DripSource;
            using osgManipulator::Translate2DDragger;            
            
            /**
             * Generates 3-dimensional openGL representations of faucets/teapots.
             */
            class FaucetGeom : public DrawableQtAdapter {
                Q_OBJECT
            public:
                explicit FaucetGeom(DripSource& dataModel);
                
                /** Create the type of dragger this geometry type supports. */
                virtual osgManipulator::Dragger* createDragger() {
                    return _dragger.get();
                }
                
                
                DripSource& getDataModel() {
                    return _dataModel;
                }
                
                void setColor(osg::Vec4 color);
                
                
                
            public slots:
                void drip();
                void setEnabled(bool enabled) ;
                void setPosition(const osg::Vec2& pos);
               
            protected:
                /** Protected to encourage use with ref_ptr. */
                virtual ~FaucetGeom();
                
            private:
                Q_DISABLE_COPY(FaucetGeom)
                void createGeom();
                
                DripSource& _dataModel;
                ref_ptr<Translate2DDragger> _dragger;
            };
        }
    }
}

#endif // __FAUCET_GEOM_H

