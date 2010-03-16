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

#include "DripSource.h"
#include <QString>

namespace ews {
    namespace app {
        namespace model {
            using ews::physics::WaveModel;
            
            DripSource::DripSource(WaveModel& model, QObject * parent)
            :  QObject(parent), _oscillator(model), _timer(), _paused(false), _enabled(false), _frequency(0) {
                
                connect(this, SIGNAL(enabledChanged(bool)), this, SLOT(updateTimer()));
                connect(this, SIGNAL(frequencyChanged(int)), this, SLOT(updateTimer()));
                connect(this, SIGNAL(drip(int)), this, SLOT(pokeOscillator()));
                connect(&_timer, SIGNAL(timeout()), this, SLOT(pulseDrip()));
                
                setPosition(osg::Vec2(0, 0));
                _oscillator.setRadius(2);
                
                
                updateTimer();
            }
            
            void DripSource::updateTimer()  {
                
                // compute delay in milliseconds from millihertz
                if(getFrequency() == 0) {
                    _timer.stop();
                    return;
                }
                
                int delay = 100000/getFrequency();
                qDebug() << "***** drip delay is: " << delay;
                
                if(_timer.interval() != delay && delay > 0) {
                    _timer.setInterval(delay);
                }
                
                if(_timer.isActive() != isEnabled()) {
                    if(isEnabled()) {
                        _timer.start();
                    }
                    else {
                        _timer.stop();
                    }
                }
                
                // A bit of a hack to get a drop immediately
                // when the timer is turned on or changed.
                if(_timer.isActive()) {
                    pulseDrip();
                }
            }
            
            void DripSource::pokeOscillator() {
                _oscillator.firePulse();
            }
            
            void DripSource::initialize(Real maxWidth, Real maxLength) {
                if (objectName() != DRIPSOURCE2) {
                    setPosition(osg::Vec2(maxWidth / 2, maxLength / 2));
                    setEnabled(true);
                }
                else {
                    setPosition(osg::Vec2(20 < maxWidth ? 20 : maxWidth / 4, 20 < maxLength ? 20 : maxLength / 4));
                    setEnabled(false);
                }
                setFrequency(0);
                setPaused(false);
                setAmplitude(50);
            }
        }
    }
}
