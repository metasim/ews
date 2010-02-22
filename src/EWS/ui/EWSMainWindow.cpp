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

#include "EWSMainWindow.h"
#include "ui_EWSMainWindow.h"

#include "WaterSurfaceGeom.h"

EWSMainWindow::EWSMainWindow(SimulationState* state, QWidget *parent)
    : QMainWindow(parent), _ui(new Ui::EWSMainWindow), _state(state)
{
    _ui->setupUi(this);
    
    
    _ui->dripSource1->setDataModel(state->dripSource1());
    _ui->dripSource2->setDataModel(state->dripSource2());
    
    
    osg::Group* root = new osg::Group;
    
    osg::ref_ptr<WaterSurfaceGeom> water = new WaterSurfaceGeom;
    
    
    root->addChild(water.get());
    
    _ui->renderer->setSceneData(root);
    
}

EWSMainWindow::~EWSMainWindow()
{
    delete _ui;
}
