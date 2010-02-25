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

#ifndef _DRIPSOURCEEDITOR_H
#define _DRIPSOURCEEDITOR_H

#include <QtGui/QWidget>
#include "DripSource.h"

namespace Ui
{
    class DripSourceEditor;
}

class DripSourceEditor : public QWidget {
    Q_OBJECT
    
public:
    DripSourceEditor(QWidget *parent = 0);
    ~DripSourceEditor();

    void setDataModel(DripSource* data);
    
public slots:
    void setEnabledState(bool state) {
        if(_dataModel) {
            _dataModel->setEnabled(state);
        }
    }
    
    void pulse() {
        if(_dataModel) {
            _dataModel->pulseDrip();
        }
    }
    
    void setAmplitude(int val) {
        if(_dataModel) {
            _dataModel->setAmplitude(val);
        }
    }
    
    void setFrequency(int val) {
        if(_dataModel) {
            _dataModel->setFrequency(val);
        }
    }

private slots:
    void throb();
    
private:
    void syncUI();
    Ui::DripSourceEditor* _ui;
    DripSource* _dataModel;
};

#endif // _DRIPSOURCEEDITOR_H