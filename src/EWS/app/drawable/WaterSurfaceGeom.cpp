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

#include <osg/Shape>
#include <osgTerrain/Terrain>
#include <osgTerrain/TerrainTile>
#include <osgTerrain/GeometryTechnique>
#include <osgTerrain/Layer>

namespace ews {
    namespace app {
        namespace drawable {
            using ews::app::model::WaveMedium;
            using osg::ref_ptr;
            
            WaterSurfaceGeom::WaterSurfaceGeom(WaveMedium& settings) : 
            DrawableQtAdapter(&settings), _settings(settings) {

                QObject::connect(&_settings, SIGNAL(sizeChanged(int,int)), this, SLOT(updateWaterGeometry()));
                QObject::connect(&_settings, SIGNAL(resolutionChanged(int)), this, SLOT(updateWaterGeometry()));
                
                updateWaterGeometry();
            }
            
            
            void WaterSurfaceGeom::updateWaterGeometry() {
                using namespace osgTerrain;
                using namespace osg;
                
                removeChildren(0, getNumChildren());
                
//                ref_ptr<Terrain> t = new Terrain();
                
                ref_ptr<TerrainTile> terrainTile = new TerrainTile;
                
                ref_ptr<Locator> locator = new Locator;
                ref_ptr<ValidDataOperator> validDataOperator = new NoDataValue(0.0);
                ref_ptr<Layer> lastAppliedLayer;
                terrainTile->setTreatBoundariesToValidDataAsDefaultValue(true);
                
                locator->setCoordinateSystemType(Locator::PROJECTED);
                locator->setTransformAsExtents(0, 0, _settings.width(), _settings.length());
                
//                ref_ptr<GeometryTechnique> geometryTechnique = new GeometryTechnique;
//                terrainTile->setTerrainTechnique(geometryTechnique.get());
                
                ref_ptr<HeightField> hf = new HeightField;
                ref_ptr<HeightFieldLayer> hfl = new HeightFieldLayer;
                hfl->setHeightField(hf.get());
                
                hfl->setLocator(locator.get());
                hfl->setValidDataOperator(validDataOperator.get());
                
//                terrainTile->setElevationLayer(hfl.get());
                
                
                addChild(terrainTile.get());
                
//                
//                
//                osg::ref_ptr<osgTerrain::HeightFieldLayer> hfl = new osgTerrain::HeightFieldLayer;
//                hfl->setHeightField(hf.get());
//                
//                hfl->setLocator(locator.get());
//                hfl->setValidDataOperator(validDataOperator.get());
//                hfl->setMagFilter(filter);
//                
//                if (offset!=0.0f || scale!=1.0f)
//                {
//                    hfl->transform(offset,scale);
//                }
//                
//                terrainTile->setElevationLayer(hfl.get());
//                
//                lastAppliedLayer = hfl.get();
//                
                
            }
            
        }
    }
}