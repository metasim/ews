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
#include <vector>
#include "EWSUtils.h"
#include "WaveModel.h"
#include "Lattice.h"

namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            using ews::physics::WaveModel;
            using ews::physics::Lattice;
            using namespace ews::util;
            
            /** Texture binding ID used here and in WaterSurfaceGeom. */
            const int TEX_ID = 0;
            
            /**
             * Callback class used by WaterSurfaceGeom to update the water surface height.
             */
            class VertexHeightUpdater :  public osg::NodeCallback  {
            public:
                VertexHeightUpdater(WaveMedium& model, Geometry* geom) :
                _model(model), _heightMap(0) {
                    
                    unsigned int gridWidth = _model.getWidth();
                    unsigned int gridLength = _model.getLength();
                    
                    ref_ptr<StateSet> state = geom->getOrCreateStateSet();
                    
                    ref_ptr<Program> program = new Program;
                    state->setAttribute(program.get());
                    
                    program->addShader(new Shader(Shader::VERTEX, loadVertexProgram()));
                    program->addShader(new Shader(Shader::FRAGMENT, loadFragmentProgram()));

                    _heightMap = new Image;
                    _heightMap->setDataVariance(Object::DYNAMIC);
                    
                    _heightMap->allocateImage(gridWidth, gridLength, 1, GL_LUMINANCE, GL_FLOAT, 1);
                    _heightMap->setInternalTextureFormat(GL_LUMINANCE32F_ARB);
                    applyHeightMap(_heightMap);

                    ref_ptr<Texture2D> tex = new Texture2D(_heightMap.get());
                    tex->setResizeNonPowerOfTwoHint(false);
                    tex->setDataVariance(Object::DYNAMIC);
                    tex->setWrap(Texture2D::WRAP_S, Texture2D::CLAMP);
                    tex->setWrap(Texture2D::WRAP_T, Texture2D::CLAMP);
                    tex->setFilter(Texture2D::MIN_FILTER, Texture2D::LINEAR);
                    tex->setFilter(Texture2D::MAG_FILTER, Texture2D::LINEAR);
                    state->setTextureAttributeAndModes(TEX_ID, tex.get(), StateAttribute::ON);

                    // Set shader variables.
                    ref_ptr<Uniform> gridSize = new Uniform("gridSize", Vec2(gridWidth, gridLength));
                    gridSize->setDataVariance(Object::STATIC);
                    state->addUniform(gridSize.get());
                                             
                    ref_ptr<Uniform> heightMapID = new Uniform("heightMap", TEX_ID);
                    heightMapID->setDataVariance(Object::STATIC);
                    state->addUniform(heightMapID.get());
                }
                
                virtual ~VertexHeightUpdater() {
                }
                    
                /**
                 *  Callback method called by the NodeVisitor when visiting a node.
                 *  TODO: Consider moving the propagate stage of this
                 *  out of this class and into something more general, such
                 *  as a parent decorator node for simulation update. See
                 *  corresponding code in FaucetGeom.
                 */
                virtual void operator()(Node* node, NodeVisitor* nv) { 
                    /** If the simulation is paused then we don't propagate
                     *  the wave model nor update the deformation texture.
                     */
                    if(!_model.isPaused()) {
                        _model.getWaveModel().propagate();
                        
                        applyHeightMap(_heightMap);
                    }
                    traverse(node,nv);
                }
                
            private:
                WaveMedium& _model;
                ref_ptr<Image> _heightMap;
                
                void applyHeightMap(Image* image) {
                    using std::vector;

                    unsigned int gridWidth = _model.getWidth();
                    unsigned int gridLength = _model.getLength();

                        
                    const Lattice& lattice = _model.getWaveModel().getLattice();
                    // The efficiency of this copy operation is very important
                    // could surely be made better.
                    for(unsigned int x = 0; x < gridWidth; x++) {
                        for(unsigned int y = 0; y < gridLength; y++) {
                            *((float*)image->data(x,y)) = (float) lattice.getValue(x, y);
                        }
                    }
                    
                    // qDebug() << "middle lattice amplitude: " << lattice.getValue(lattice.getWidth()/2, lattice.getLength()/2);

                    image->dirty();
                }
                
                static QString loadTextResource(const char* resource) {
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
                    
                    return QString(bytes);
                }
                
                static const char* loadFragmentProgram() {
                    static QString data;
                    if(!data.length()) {
                        data = loadTextResource(":text/waves.fs");
                    }
                    return qPrintable(data);
                }
                static const char* loadVertexProgram() {
                    static QString data;
                    if(!data.length()) {
                        data = loadTextResource(":text/waves.vs");
                    }
                    return qPrintable(data);
                }
            } ;
        }
    }
}

#endif // __VERTEX_HEIGHT_UPDATER_H
