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


DripSource::DripSource(QObject * parent)
: QObject(parent), _enabled(false), _amplitude(50), _frequency(1000), _timer(this)
{
    connect(this, SIGNAL(enabledChanged(bool)), this, SLOT(updateTimer()));
    connect(this, SIGNAL(frequencyChanged(int)), this, SLOT(updateTimer()));
    
    connect(&_timer, SIGNAL(timeout()), this, SLOT(pulseDrip()));
    
    updateTimer();
}

void DripSource::updateTimer() 
{
    
    // compute delay in milliseconds from millihertz
    int delay = (1000*1000)/_frequency;
    
    if(_timer.interval() != delay && delay > 0) {
        _timer.setInterval(delay);
    }
    
    if(_timer.isActive() != enabled()) {
        if(enabled()) {
            _timer.start();
        }
        else {
            _timer.stop();
        }
    }
}
