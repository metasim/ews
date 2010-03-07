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

#ifndef __BARRIEREDITOR_H
#define __BARRIEREDITOR_H

#include <QtGui/QWidget>
#include <QDataWidgetMapper>
#include "BarrierSet.h"

/** Forward declaration of UI implementation class. */

namespace Ui
{
    class BarrierEditorForm;
}

namespace ews {
    namespace app {
        namespace widget {
            
            using ews::app::model::BarrierSet;
            using ews::app::model::Barrier;
            
            class BarrierEditor : public QWidget {
                Q_OBJECT
                
            public:
                BarrierEditor(QWidget *parent = 0);
                ~BarrierEditor();
                
                void setDataModel(BarrierSet* barriers);

            public slots:
                void addBarrier();
                void removeBarrier();
                void updateEnabled();
                /** Update UI after the table seletion changes. */
                void updateOnSelection();
                
            private:
                Barrier::NumSlits numSlitsSelected() const;
                void select(Barrier* barrier);
                Barrier* selectedBarrier() const;

                Ui::BarrierEditorForm* _ui;
                QDataWidgetMapper _mapper;
                BarrierSet* _dataModel;
            };
        }
    }
}

#endif // __BARRIEREDITOR_H
