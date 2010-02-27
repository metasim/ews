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

namespace ews {
    namespace app {
        namespace drawable {
            using ews::app::model::WaveMedium;
            using osg::ref_ptr;
            
            

            
            class UniformVarying : public osg::Uniform::Callback
            {
                virtual void operator () (osg::Uniform* uniform, osg::NodeVisitor* nv)
                {
                    const osg::FrameStamp* fs = nv->getFrameStamp();
                    float value = fs->getSimulationTime();
                    uniform->set(osg::Vec4(value,-value,value,value));
                }
            };
            
            
            WaterSurfaceGeom::WaterSurfaceGeom(WaveMedium& settings) : 
            DrawableQtAdapter(&settings), _settings(settings) {

                QObject::connect(&_settings, SIGNAL(sizeChanged(int,int)), this, SLOT(updateWaterGeometry()));
                QObject::connect(&_settings, SIGNAL(resolutionChanged(int)), this, SLOT(updateWaterGeometry()));
                
                updateWaterGeometry();
            }
            
            
            ///////////////////////////////////////////////////////////////////
            // vertex shader using just Vec4 coefficients
            char vertexShaderSource_simple[] = 
            "uniform vec4 coeff; \n"
            "\n"
            "void main(void) \n"
            "{ \n"
            "\n"
            "    // gl_TexCoord[0] = gl_Vertex; \n"
            "    vec4 vert = gl_Vertex; \n"

            
            
            "    vert.z = (sin(coeff[0] + (gl_Vertex.x/5.0)) + sin(coeff[1] + (gl_Vertex.y/4.0))) * 2.5; "
            
            "    gl_Position = gl_ModelViewProjectionMatrix * vert;\n"
            "    gl_FrontColor = gl_Color\n;"
            "}\n";
            
            void WaterSurfaceGeom::updateWaterGeometry() {

                using namespace osg;
                
                removeChildren(0, getNumChildren());
                
                ref_ptr<Geode> geode = new Geode;
                {
                    // Turn on wireframe mode.
                    ref_ptr<osg::StateSet> state = geode->getOrCreateStateSet();
                
                    // Create a PolygonMode attribute 
                    ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE ); 
                    // Force wireframe rendering. 
                    state->setAttributeAndModes(pm, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE); 
                }
                addChild(geode.get());

                
                
                ref_ptr<Geometry> geom = new Geometry;
                geode->addDrawable(geom.get());
                {
                    // Set temporary color
                    ref_ptr<Vec3Array> colors = new osg::Vec3Array(1);
                    (*colors)[0] = Vec3(1, 0, 1);
                    geom->setColorArray(colors.get());
                    geom->setColorBinding(Geometry::BIND_OVERALL);
                }

                {
                    ref_ptr<StateSet> state = geom->getOrCreateStateSet();
                
                    ref_ptr<Program> program = new Program;
                    state->setAttribute(program.get());
                
                    ref_ptr<Shader> vp = new Shader(osg::Shader::VERTEX, vertexShaderSource_simple);
                    program->addShader(vp.get());
                    
                    ref_ptr<Uniform> coeff = new Uniform("coeff",osg::Vec4(1.0,1.0f,1.0f,1.0f));
                    
                    state->addUniform(coeff.get());
                    
                    coeff->setUpdateCallback(new UniformVarying);
                    coeff->setDataVariance(osg::Object::DYNAMIC);
                    
                    state->setDataVariance(osg::Object::DYNAMIC);
                }
                
                unsigned int rez = _settings.latticeDivisionsPerCentimeter();
                unsigned int num_x = _settings.width() * rez;
                unsigned int num_y = _settings.length() * rez;
                
                // Code below adapted from demo file osgparametric.cpp
                ref_ptr<Vec3Array> vertices = new Vec3Array( num_x * num_y );
                
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
            }
            
        }
    }
}