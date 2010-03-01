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

#ifndef __VERTEX_HEIGHT_UPDATER_H
#define __VERTEX_HEIGHT_UPDATER_H

#include <osg/Drawable>
#include <osg/Geometry>
#include <QByteArray>


// http://www.ozone3d.net/tutorials/mesh_deformer_p2.php

namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            static const int HEIGHT_ARRAY_ATTR_ID = 14; 

            
            class VertexHeightUpdater : public Drawable::UpdateCallback {
            public:
                VertexHeightUpdater(Geometry* geom, unsigned int width, unsigned int length) :
                _gridWidth(width), _gridLength(length) {
                    Array* va = geom->getVertexArray();
                    unsigned int totalVerts = va->getNumElements();
                    ref_ptr<StateSet> state = geom->getOrCreateStateSet();
                    
                    ref_ptr<Program> program = new Program;
                    state->setAttribute(program.get());
                    
                    program->addShader(new Shader(osg::Shader::VERTEX, loadVertexProgram()));
                    program->addShader(new osg::Shader(osg::Shader::FRAGMENT, loadFragmentProgram()));

                    
                    state->setDataVariance(osg::Object::DYNAMIC);
                    
                    
                    ref_ptr<FloatArray> attrib = new FloatArray(totalVerts); 
                    attrib->setDataVariance(osg::Object::DYNAMIC);
                    
                    // (Fill attrib with per-vertex attributes) 
                    geom->setVertexAttribArray(HEIGHT_ARRAY_ATTR_ID, attrib.get()); 
                    geom->setVertexAttribBinding(HEIGHT_ARRAY_ATTR_ID, osg::Geometry::BIND_PER_VERTEX); 
                    program->addBindAttribLocation("height", HEIGHT_ARRAY_ATTR_ID); 
                    
                }
                
                virtual ~VertexHeightUpdater() {
                }
                
                inline virtual void update (osg::NodeVisitor* nv, osg::Drawable* d) {

                    Geometry* geometry = d->asGeometry();
                    Array* data = geometry->getVertexAttribArray(HEIGHT_ARRAY_ATTR_ID);
                    osg::FloatArray* floats = dynamic_cast<FloatArray*>(data);
                    if(!floats) return;
                    
                    const osg::FrameStamp* fs = nv->getFrameStamp();
                    float value = fs->getSimulationTime();
                    int a = 2;
                    int i = 0;
                    for(unsigned int y = 0; y < _gridLength; y++) {
                        for(unsigned int x = 0; x < _gridWidth; x++) {
                            float px = (x - _gridWidth/2.0f);
                            float py = (y - _gridLength/2.0f);
                            float p = sqrtf(px*px + py*py);
                            (*floats)[i++] = a * sinf(p + value*2);
                        }
                    }
                    
                    data->dirty();
                }
                
            private:
                unsigned int _gridWidth;
                unsigned int _gridLength;
                
                static char* loadTextResource(const char* resource) {
                    QFile progResource(resource);
                    if (!progResource.open(QIODevice::ReadOnly|QIODevice::Text)) {
                        qWarning() << "Couldn't find" << resource;
                        return 0;
                    }
                    else {
                        qDebug() << "Loaded" << resource;
                    }
                    
                    QByteArray bytes = progResource.readAll();
                    progResource.close();
                    
                    char* retval = new char[bytes.size()];
                    qstrcpy(retval, bytes.constData());
                    return retval;
                }
                
                static const char* loadFragmentProgram() {
                    static char* data = 0;
                    if(!data) {
                        data = loadTextResource(":text/waves.fs");
                    }
                    return data;
                }
                static const char* loadVertexProgram() {
                    static char* data = 0;
                    if(!data) {
                        data = loadTextResource(":text/waves.vs");
                    }
                    return data;
                }
                
            } ;
        }
    }
}

#endif // __VERTEX_HEIGHT_UPDATER_H
