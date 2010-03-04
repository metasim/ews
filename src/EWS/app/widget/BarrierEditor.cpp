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

#include "BarrierEditor.h"
#include "ui_BarrierEditor.h"

namespace ews {
    namespace app {
        namespace widget {
            
            using ews::app::model::Barrier;
            
            BarrierEditor::BarrierEditor(QWidget* parent) 
            : QWidget(parent), _ui(new Ui::BarrierEditorForm) {
                _ui->setupUi(this);   
                
            }
            
            BarrierEditor::~BarrierEditor() {
                _dataModel = NULL; // We don't own the data model
                delete _ui;
            }

            void BarrierEditor::setDataModel(Barrier* barrier) {
                _dataModel = barrier;
            }
            
            void BarrierEditor::syncUI() {

            }
        }
    }
}