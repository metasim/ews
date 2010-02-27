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

#include <QtCore>
#include "DrawableFactory.h"
#include "DripSource.h"
#include "FaucetGeom.h"
#include "WaveMedium.h"
#include "WaterSurfaceGeom.h"

namespace ews {
    namespace app {
        namespace drawable {
            using namespace ews::app::model;
            
            DrawableFactory& DrawableFactory::instance() {
                static DrawableFactory singleton;
                return singleton;
            }
            
            
            osg::Node* DrawableFactory::createDrawableFor(QObject& data)  {
                // Currently only a loose mapping between object data and drawable type is
                // maintained, hense the switching on type.
                QString name(data.metaObject()->className());
                qDebug() << "Selecting drawable for" << name;
                if(data.inherits(DripSource::staticMetaObject.className())) {
                    DripSource* source = qobject_cast<DripSource*>(&data);
                    FaucetGeom* geom = new FaucetGeom(*source);
                    return geom;
                }
                else if(data.inherits(WaveMedium::staticMetaObject.className())) {
                    WaveMedium* waves = qobject_cast<WaveMedium*>(&data);
                    WaterSurfaceGeom* geom = new WaterSurfaceGeom(*waves);
                    return geom;
                }
                    
                qWarning() << "No drawable found for" << name;
                return NULL;
            }
        }
    }
}

