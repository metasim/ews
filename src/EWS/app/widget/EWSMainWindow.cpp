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

#include "BarrierEditor.h"

#include "DrawableFactory.h"

#include "ui_EWSMainWindow.h"
#include <QMessageBox>
#include <QDesktopServices>
#include "EWSUtils.h"
#include "EWSVersion.h"
#include "EWSDefine.h"
#include "AmplitudePlot.h"
#include "PointSampler.h"
#include "Preferences.h"

namespace ews {
    namespace app {
        namespace widget {
            using ews::Uint;
            using namespace ews::app::drawable;
            using namespace ews::app::model;
            
            EWSMainWindow::EWSMainWindow(SimulationState* state, QWidget *parent) 
            : QMainWindow(parent), _ui(new Ui::EWSMainWindowForm), _state(state) {
                _ui->setupUi(this);
                
                // Config actions
                _ui->actionPause->setEnabled(false);
                _ui->actionRun->setEnabled(true);
                
                // Config drip editors
                _ui->dripSource1->setDataModel(&state->getDripSource1());
                _ui->dripSource2->setDataModel(&state->getDripSource2());
                
                // Config barrier editor.
                _ui->barrierEditor->setDataModel(&state->getBarriers());
                
                // Config detector editor.
                _ui->detectorEditor->setDataModel(&state->getSamplers());
                
                // Config 3-D view.
                _sceneRoot = new SceneRoot(this);
                // renderer is an instance (the only one) of QOSGWidget
                _ui->renderer->setSceneData(_sceneRoot);
                // Setup sync between model and renderer.
                QObject::connect(_state, SIGNAL(objectAdded(QObject&)), _sceneRoot, SLOT(addDrawableFor(QObject&)));
                QObject::connect(_state, SIGNAL(objectRemoved(QObject&)), _sceneRoot, SLOT(removeDrawableFor(QObject&)));
                
                // Setup sync between samplers and plot.
                QObject::connect(&_state->getSamplers(), SIGNAL(samplerAdded(int,PointSampler*)), 
                                 _ui->amplitudePlot, SLOT(addSampleSource(int, PointSampler*)));
                QObject::connect(&_state->getSamplers(), SIGNAL(samplerRemoved(int,PointSampler*)), 
                                 _ui->amplitudePlot, SLOT(removeSampleSource(int, PointSampler*)));

                // Sync setup between 
                QObject::connect(&_state->getBarriers(), SIGNAL(sizeChanged(Uint)), this, SLOT(updateMenusEnabledState()));
            }
            
            EWSMainWindow::~EWSMainWindow() {
                _ui->renderer->setSceneData(NULL);
                delete _ui;
            }
            
            /** Perform post realization initialization() */
            void EWSMainWindow::init() {
                // Force synchronization between model and listeners.
                _state->emitSignalsForDefaults();
                
                // Make sure all consituents know we're getting ready to start.
                reset();
                
                // Fix menu state.
                updateMenusEnabledState();
                
                // Start simulation.
                start();
            }

            void EWSMainWindow::start() {
                _state->setPaused(false);
                _ui->actionPause->setEnabled(true);
                _ui->actionRun->setEnabled(false);
                
            }
            
            void EWSMainWindow::stop() {
                _state->setPaused(true);
                _ui->actionPause->setEnabled(false);
                _ui->actionRun->setEnabled(true);
            }
            
            void EWSMainWindow::reset() {
                _state->reset();
                _ui->dripSource1->syncUI();
                _ui->dripSource2->syncUI();
                _ui->renderer->homePosition();
                _ui->amplitudePlot->reset();
                
                // If paused we fire a run action trigger
                // for convenience to start the simulation again
                // with the default state (and update the button enabled state).

                if(_state->isPaused()) {
                    _ui->actionRun->trigger();
                }
                
                updateMenusEnabledState();       
            }
            
            /** Detect when we should perform post realization initialization() */
            bool EWSMainWindow::event(QEvent* event) {
                int retval = QWidget::event(event);

                if (event->type() == QEvent::Polish) {
                    init();
                }
                
                return retval;
            }
            
            void EWSMainWindow::updateMenusEnabledState() {
                _ui->actionAddBarrier->setEnabled(!_state->getBarriers().isFull());
                _ui->actionRemoveBarrier->setEnabled(_state->getBarriers().size() > 0);
            }
            
            Uint EWSMainWindow::getInterFrameDelay() const {
                return _ui->renderer->getInterFrameDelay();
            }
            
            void EWSMainWindow::setInterFrameDelay(Uint delayMS) {
                _ui->renderer->setInterFrameDelay(delayMS);
            }
            
            /** Prefs dialog. */
            void EWSMainWindow::preferences() {
                Preferences* p = new Preferences(this);
                p->show();
            }
            
            /** Show the about box. */
            void EWSMainWindow::about() {
                using ews::util::loadTextResource;
                QMessageBox::about(this, "About...", loadTextResource(":/text/about"));
            }
            
            /** Request the OS open the configured project site. */
            void EWSMainWindow::projectWebsite() {
                QUrl url(EWS_WEBSITE,QUrl::TolerantMode);
                qDebug() << "Opening" << url;
                QDesktopServices::openUrl(url);
            }

        }
    }
}

