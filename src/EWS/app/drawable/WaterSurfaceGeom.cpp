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
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PolygonMode>
#include <osg/Material>
#include "VertexHeightUpdater.h"

namespace ews {
    namespace app {
        namespace drawable {
            using ews::app::model::WaveMedium;
            using osg::ref_ptr;
            using namespace osg;

            WaterSurfaceGeom::WaterSurfaceGeom(WaveMedium& settings) : 
            DrawableQtAdapter(&settings), _settings(settings) {

                QObject::connect(&_settings, SIGNAL(sizeChanged(int,int)), this, SLOT(updateWaterGeometry()));
                QObject::connect(&_settings, SIGNAL(resolutionChanged(int)), this, SLOT(updateWaterGeometry()));
                
                osg::StateSet* state = getOrCreateStateSet(); 
                osg::ref_ptr<Material> mat = new Material; 
                mat->setDiffuse(Material::FRONT,
                                Vec4( .2f, .2f, .9f, 1.f ) ); 
                mat->setSpecular(Material::FRONT,
                                 Vec4( 8.f, 8.f, 1.f, 1.f ) ); 
                mat->setShininess(Material::FRONT, 40.f ); 
                mat->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
                state->setAttribute( mat.get() );
                
                updateWaterGeometry();
            }

            
            void WaterSurfaceGeom::updateWaterGeometry() {

                removeChildren(0, getNumChildren());
                
                
                unsigned int rez = _settings.latticeDivisionsPerCentimeter();
                unsigned int num_x = _settings.width() * rez;
                unsigned int num_y = _settings.length() * rez;
                unsigned int totalVerts = num_x * num_y;
                
                ref_ptr<Geode> geode = new Geode;
                if(true) {
                    // Turn on wireframe mode.
                    ref_ptr<osg::StateSet> state = geode->getOrCreateStateSet();
                
                    // Create a PolygonMode attribute 
                    ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE ); 
                    // Force wireframe rendering. 
                    state->setAttributeAndModes(pm.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE); 
                }
                
                addChild(geode.get());

                ref_ptr<Geometry> geom = new Geometry;
                geode->addDrawable(geom.get());
                
                // Code below adapted from demo file osgparametric.cpp
                ref_ptr<Vec3Array> vertices = new Vec3Array( totalVerts );
                
                float dx = 1/(float) rez;
                float dy = dx;
                osg::Vec3 row(-_settings.width()/2.0f, -_settings.length()/2.0f,0.0f);
                
                // Concice way of allocating grid vertices.
                unsigned int vert_no = 0;
                unsigned int iy;
                for(iy = 0; iy<num_y; ++iy) {
                    osg::Vec3 column = row;
                    for(unsigned int ix = 0 ; ix < num_x; ++ix) {
                        (*vertices)[vert_no++] = column;
                        column.x() += dx;
                    }        
                    row.y() += dy;
                }
                
                // Add the verticies to the geometry, and tag them as 
                // being stored as a VBO.
                geom->setVertexArray(vertices.get());
                ref_ptr<VertexBufferObject> vbObject = new VertexBufferObject;
                vertices->setVertexBufferObject(vbObject.get());
                
                
                ref_ptr<Vec3Array> n = new Vec3Array; 
                geom->setNormalArray(n.get()); 
                geom->setNormalBinding(Geometry::BIND_OVERALL); 
                n->push_back(Vec3( 0.f, 1.f, 0.f ));
                
                // Create the vertex mesh as triangle strips, stored as an EBO
                ref_ptr<ElementBufferObject> ebo = new ElementBufferObject;
                for(iy=0; iy<num_y-1; ++iy) {
                    unsigned int element_no = 0;
                    ref_ptr<DrawElementsUInt> elements = new DrawElementsUInt(GL_TRIANGLE_STRIP, num_x*2);
                    unsigned int index = iy * num_x;
                    for(unsigned int ix = 0; ix<num_x; ++ix) {
                        (*elements)[element_no++] = index + num_x;
                        (*elements)[element_no++] = index++;
                    }
                    geom->addPrimitiveSet(elements.get()); 
                    
                    if (ebo) elements->setElementBufferObject(ebo.get());   
                }
                
                geom->setUseVertexBufferObjects(true);
                
                
                // Register the delegate responsible for updating hight and 
                // doing shading.
                geom->setUpdateCallback(new VertexHeightUpdater(geom, num_x, num_y));
            }
        }
    }
}