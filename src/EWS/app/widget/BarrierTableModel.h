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

#ifndef __BARRIER_TABLE_MODEL
#define __BARRIER_TABLE_MODEL
#include "BarrierSet.h"
#include <QAbstractTableModel>

namespace ews {
    namespace app {
        namespace widget {
            using ews::app::model::BarrierSet;
            using ews::app::model::Barrier;
            
            class BarrierTableModel : public QAbstractTableModel {
                Q_OBJECT
            public:
                BarrierTableModel(BarrierSet* barriers) 
                : QAbstractTableModel(), _barriers(barriers) {
                    connect(barriers, SIGNAL(barrierAdded(int,Barrier*)), this, SLOT(rowsAdded(int)));
                    connect(barriers, SIGNAL(barrierRemoved(int,Barrier*)), this, SLOT(rowsRemoved(int)));
                }
                
                virtual ~BarrierTableModel() {
                    _barriers = NULL;
                }
                
                virtual int rowCount(const QModelIndex &parent = QModelIndex()) const {
                    return _barriers->size();
                }
                virtual int columnCount(const QModelIndex &parent = QModelIndex()) const {
                    return 2;
                }
                
                virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
                    int i = index.row();
                    int j = index.column();
                    Barrier* b = _barriers->barrierAt(i);
                    
                    if(role == Qt::DisplayRole) {
                        switch(j) {
                            case 0:
                                return QVariant(b->isEnabled());
                            case 1:
                                return QString("foo: " + b->objectName());
                        }
                    }
                    
                    return QVariant();
                }
                
                
                virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const {
                    if(role == Qt::DisplayRole && orientation == Qt::Horizontal) {
                        switch(section) {
                            case 0:
                                return QString(tr("Enabled"));
                            case 1:
                                return QString(tr("Name"));
                        }
                    }                    
                    return QVariant();
                }
                
            public slots:
                bool rowsAdded(int position) {
                    beginInsertRows(QModelIndex(), position, position);
                    // Rows already added in model.
                    endInsertRows();
                    return true;
                }
                bool rowsRemoved(int position) {
                    beginRemoveRows(QModelIndex(), position, position);
                    // Rows already removed in model.
                    endRemoveRows();
                    return true;
                }
                
            private:
                Q_DISABLE_COPY(BarrierTableModel);
                BarrierSet* _barriers;
                
            };
        }
    }
}

#endif // __BARRIER_TABLE_MODEL
