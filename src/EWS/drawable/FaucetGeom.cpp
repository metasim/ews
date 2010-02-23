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

#include <osg/Geode>
#include <osg/Drawable>
#include <osg/PolygonMode>
#include "FaucetGeom.h"
#include "demo/Teapot.h"

class FaucetDrawCallback : public osg::Drawable::DrawCallback {
    virtual void drawImplementation (osg::RenderInfo&, const osg::Drawable*) const;
};
void FaucetDrawCallback::drawImplementation (osg::RenderInfo& ri, const osg::Drawable* d) const
{
    glColor3f(0, 0, 1);
    d->drawImplementation(ri);
}

FaucetGeom::FaucetGeom(DripSource& settings) : osg::Group(), _settings(settings)
{
    osg::Geode* geode = new osg::Geode;
    addChild(geode);
    
    osg::Drawable* d = new Teapot;
    d->setDrawCallback(new FaucetDrawCallback);
    geode->addDrawable(d);

    osg::StateSet* state = geode->getOrCreateStateSet();
    
    // Create a PolygonMode attribute 
    osg::PolygonMode* pm = new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE ); 
    // Force wireframe rendering. 
    state->setAttributeAndModes(pm, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE); 
    
//    QObject::connect(&settings, SIGNAL(drip(int)), this, SLOT(drip(int)));
}


FaucetGeom::~FaucetGeom()
{
    
}


void FaucetGeom::drip(int amplitude) 
{
    qDebug() << "in geom drip";
}
