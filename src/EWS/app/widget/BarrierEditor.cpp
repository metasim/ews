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

#include "Barrier.h"
#include "BarrierEditor.h"
#include "BarrierTableModel.h"

#include <QTableView>
#include <QModelIndex>
#include <QItemDelegate>
#include <QItemSelectionModel>
#include "ui_BarrierEditor.h"

namespace ews {
    namespace app {
        namespace widget {
            
            using ews::app::model::BarrierSet;
            using ews::app::model::Barrier;
            
            class MapperDelegate : public QItemDelegate {
//                Q_OBJECT
            public:
                MapperDelegate(QObject* parent) : QItemDelegate(parent) {}
                virtual ~MapperDelegate() {}
                
                virtual void setEditorData(QWidget* editor,
                                   const QModelIndex &index) const {

                    QTRACE;
                    qDebug() << *editor << index;
                    QItemDelegate::setEditorData(editor, index);
                }
                
                virtual void setModelData(QWidget* editor,
                                  QAbstractItemModel* model,
                                  const QModelIndex& index) const {
                    
                    QTRACE;
                    qDebug() << (*editor) << index << model;
                    
                    
                    QItemDelegate::setModelData(editor, model, index);
                }
                
            private:
                Q_DISABLE_COPY(MapperDelegate)
            };
            
            BarrierEditor::BarrierEditor(QWidget* parent) 
            : QWidget(parent), _ui(new Ui::BarrierEditorForm), _mapper(this) {
                _ui->setupUi(this);   
                _mapper.setItemDelegate(new MapperDelegate(this));
                _mapper.addMapping(_ui->slitWidth, BarrierTableModel::SLIT_WIDTH);
                _mapper.addMapping(_ui->slitSeparation, BarrierTableModel::SLIT_SEPARATION);
                _mapper.setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
                updateEnabled();
            }
            
            BarrierEditor::~BarrierEditor() {
                _dataModel = NULL; // We don't own the data model
                delete _ui;
            }

            void BarrierEditor::setDataModel(BarrierSet* barriers) {
                _dataModel = barriers;
                
                QAbstractItemModel* model = _ui->barrierTable->model();
                if(model) {
                    _ui->barrierTable->setModel(NULL);
                    QItemSelectionModel* sm = _ui->barrierTable->selectionModel();
                    if(sm) {
                        sm->disconnect(this);
                        delete sm;
                    }
                    
                    delete model;
                }
                
                if(_dataModel) {
                    QAbstractItemModel* newModel = new BarrierTableModel(_dataModel);
                    
                    _ui->barrierTable->setModel(newModel);
                    
                    
                    // Hide columns we don't want in the table.
//                    for(unsigned int c = BarrierTableModel::NAME + 1; c < BarrierTableModel::NUM_COLS; c++) {
//                        _ui->barrierTable->setColumnHidden(c, true);
//                    }
                    
                    _mapper.setModel(newModel);
                    _mapper.toFirst();

                    connect(_ui->barrierTable->selectionModel(), 
                            SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
                            this, SLOT(updateOnSelection()));
                }
                
                updateEnabled();
            }
            
            void BarrierEditor::select(Barrier* barrier) {
                int i = _dataModel->indexOf(barrier);
                if(i >= 0) {
                    QModelIndex idx = _ui->barrierTable->model()->index(i, 0);
                    _ui->barrierTable->setCurrentIndex(idx);
                }
            }
            
            Barrier* BarrierEditor::selectedBarrier() const {
                QModelIndex idx = _ui->barrierTable->currentIndex();
                return idx.row() >= 0 ? _dataModel->barrierAt(idx.row()) : NULL;
            }
            
            void BarrierEditor::updateOnSelection() {
                QModelIndex idx = _ui->barrierTable->currentIndex();
                qDebug() << "selected barrier:" << idx;
                if(idx.row() >= 0) {
                    _mapper.setCurrentIndex(idx.row());
                }

                updateEnabled();
            }

            void BarrierEditor::addBarrier() {
                Barrier* b = _dataModel->createBarrier();
                select(b);
                _ui->barrierTable->resizeRowsToContents();
            }
            
            void BarrierEditor::removeBarrier() {
                Barrier* b = selectedBarrier();
                _dataModel->removeBarrier(b);
                updateEnabled();
            }
            
            Barrier::NumSlits BarrierEditor::numSlitsSelected() const {
                if(_ui->zeroSlits->isChecked()) {
                    return Barrier::ZERO;
                }
                else if(_ui->oneSlit->isChecked()) {
                    return Barrier::ONE;
                }
                else if(_ui->twoSlits->isChecked()) {
                    return Barrier::TWO;
                }
                return Barrier::ZERO;                   
            }
            
            void BarrierEditor::updateEnabled() {
                bool canEdit = false;
                if(_ui->barrierTable->selectionModel()) {
                    canEdit = _ui->barrierTable->selectionModel()->hasSelection();
                }
                
                _ui->removeBarrier->setEnabled(canEdit);
                _ui->slitNumSelect->setEnabled(canEdit);
                
                Barrier::NumSlits num = numSlitsSelected();
                
                bool canEditSlit = canEdit && num != Barrier::ZERO;
                _ui->slitWidth->setEnabled(canEditSlit);
                _ui->slitSeparation->setEnabled(canEditSlit);
            }

        }
    }
}