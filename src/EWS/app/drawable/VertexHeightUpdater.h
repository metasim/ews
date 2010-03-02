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
#include <osg/Texture2D>
#include <osg/Image>
#include <osgDB/ReadFile>
#include <QByteArray>


namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;

            /** Texture binding ID used here and in WaterSurfaceGeom. */
            const int TEX_ID = 0;
            
            class VertexHeightUpdater : public NodeCallback {
            public:
                VertexHeightUpdater(Geometry* geom, unsigned int width, unsigned int length) :
                _gridWidth(width), _gridLength(length), _heightMap(0) {
                    ref_ptr<StateSet> state = geom->getOrCreateStateSet();
                    // state->setDataVariance(Object::DYNAMIC);
                    
                    ref_ptr<Program> program = new Program;
                    state->setAttribute(program.get());
                    
                    program->addShader(new Shader(Shader::VERTEX, loadVertexProgram()));
                    program->addShader(new Shader(Shader::FRAGMENT, loadFragmentProgram()));

                    _heightMap = new Image;
                    _heightMap->setDataVariance(Object::DYNAMIC);
                    
                    if(false) {
                        // Load the texture image 
                        _heightMap = 
                            osgDB::readImageFile("/MyHome/sfitch/Coding/libs/OpenSceneGraph-Data/Images/osg256.png"); 
                    }
                    else {
                        // heightMap->setResizeNonPowerOfTwoHint(false);
                        _heightMap->allocateImage(_gridWidth, _gridLength, 1, GL_LUMINANCE, GL_FLOAT, 1);
                        _heightMap->setInternalTextureFormat(GL_LUMINANCE32F_ARB);
                        applyHeightMap(_heightMap, 0);
                    }
                    
                    ref_ptr<Texture2D> tex = new Texture2D(_heightMap.get());
                    tex->setDataVariance(Object::DYNAMIC);
                    tex->setWrap(Texture2D::WRAP_S, Texture2D::REPEAT);
                    tex->setWrap(Texture2D::WRAP_T, Texture2D::REPEAT);
                    tex->setFilter(Texture2D::MIN_FILTER, Texture2D::LINEAR);
                    tex->setFilter(Texture2D::MAG_FILTER, Texture2D::LINEAR);
                    state->setTextureAttributeAndModes(TEX_ID, tex.get(), StateAttribute::ON);

                    // Set shader variables.
                    ref_ptr<Uniform> gridSize = new Uniform("gridSize", Vec2(_gridWidth, _gridLength));
                    gridSize->setDataVariance(Object::STATIC);
                    state->addUniform(gridSize.get());
                                             
                    // TODO figure out how to extract the right texture ID out of
                    // Texture2D. It's in the TextureObject, but private.
                    ref_ptr<Uniform> heightMapID = new Uniform("heightMap", TEX_ID);
                    heightMapID->setDataVariance(Object::STATIC);
                    state->addUniform(heightMapID.get());
                }
                
                virtual ~VertexHeightUpdater() {
                }
                    
                /** Callback method called by the NodeVisitor when visiting a node.*/
                virtual void operator()(Node* node, NodeVisitor* nv) { 
                    const osg::FrameStamp* fs = nv->getFrameStamp();
                    float value = fs->getSimulationTime();
                    
                    applyHeightMap(_heightMap, value);
                    
                    traverse(node,nv);
                }
                
            private:
                unsigned int _gridWidth;
                unsigned int _gridLength;
                ref_ptr<Image> _heightMap;
                
                void applyHeightMap(Image* image, float time) {
                    int a = 1;
                    for(unsigned int y = 0; y < _gridLength; y++) {
                        for(unsigned int x = 0; x < _gridWidth; x++) {
                            float px = (x - _gridWidth/2.0f);
                            float py = (y - _gridLength/2.0f);
                            float p = sqrtf(px*px + py*py);
                            *((float*)image->data(x,y)) = a * sinf(p/8.0 - time*4);
                        }
                    }
                    
                    image->dirty();
                }
                
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
