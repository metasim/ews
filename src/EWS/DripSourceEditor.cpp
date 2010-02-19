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

#include "DripSourceEditor.h"
#include "ui_DripSourceEditor.h"


DripSourceEditor::DripSourceEditor(QWidget* parent) : QWidget(parent), 
_ui(new Ui::DripSourceEditor), _dataModel(NULL) 
{
    _ui->setupUi(this);   
    
}

DripSourceEditor::~DripSourceEditor() 
{
    _dataModel = NULL;
    delete _ui;
}


void DripSourceEditor::throb() {
    if(_dataModel) {
        
        bool on = false;
        
        if(_dataModel->enabled()) {
            on = !_ui->throbber->isEnabled();
        }
        
        _ui->throbber->setEnabled(on);
    }
}

void DripSourceEditor::setDataModel(DripSource* data) 
{
    if(_dataModel) {
        _dataModel->disconnect(this);
    }
    
    _dataModel = data;
    
    if(_dataModel) {
        connect(_dataModel, SIGNAL(drip(int)), this, SLOT(throb()));
    }
            
    syncUI();
}


void DripSourceEditor::syncUI() 
{
    if(_dataModel) {
        blockSignals(true);
        _ui->enabled->setChecked(_dataModel->enabled());
        _ui->frequency->setValue(_dataModel->frequency());
        _ui->amplitude->setValue(_dataModel->amplitude());
        blockSignals(false);
    }
}