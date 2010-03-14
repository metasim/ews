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

#ifndef DripSource_H
#define DripSource_H

#include <QtCore>
#include <QtCore/QTimer>
#include <osg/Geometry>
#include <osg/Vec2>
#include "WaveModel.h"
#include "Oscillator.h"
#include "EWSDefine.h"

namespace ews {
    namespace app {
        namespace model {
            const QString DRIPSOURCE1("dripSource1");
            const QString DRIPSOURCE2("dripSource2");

            using ews::physics::WaveModel;
            using ews::physics::Oscillator;
            using osg::Vec2;
            
            class DripSource : public QObject {
                
                Q_OBJECT
                Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
                Q_PROPERTY(bool paused READ isPaused WRITE setPaused)
                Q_PROPERTY(Uint frequency READ getFrequency WRITE setFrequency)
                Q_PROPERTY(Uint amplitude READ getAmplitude WRITE setAmplitude)
                Q_PROPERTY(Vec2 position READ getPosition WRITE setPosition)
                
            public:
                /**
                 * Standard ctor.
                 */
                explicit DripSource(WaveModel& model, QObject * parent = 0);
                
                /**
                 * Determine if this source is enabled or not.
                 */
                bool isEnabled() const {
//                    return _oscillator.getOscillateStatus();
                    return _enabled;
                }
                
                
                /**
                 * Get the drip frequency in millihertz. 
                 */
                Uint getFrequency() const {
                    return _frequency;
//                    return (Uint) (1000.0/_oscillator.getPeriod());
                }
                
                
                /**
                 * Get the amplitude, (0, 100]
                 */
                Uint getAmplitude() const {
                    return (int) (_oscillator.getAmplitude() / ews::physics::MAX_AMPLITUDE * 100);
                }
                
                /**
                 * Get the position of the drip source in the X/Y plane.
                 * @return 2D location.
                 */
                osg::Vec2 getPosition() const {
                    return osg::Vec2(_oscillator.x(), _oscillator.y());
                }
                
                /**
                 * Get the physics simulation peer object.
                 */
                Oscillator& getOscillator() {
                    return _oscillator;
                }
                
                bool isPaused() const {
                    return _paused;
                }
                
                /**
                 * Initializes (or resets) the drip soruce according to its objectName.
                 */
                void initialize(Real maxWidth, Real maxLength);
                    
                
            public slots:
                
                /**
                 * Set the enabled state of this drip source.
                 */
                void setEnabled(bool state) {
//                    _oscillator.setOscillateStatus(state);
                    _enabled = state;
                    emit enabledChanged(state);
                }
                
                /**
                 * Set the paused state.
                 */
                void setPaused(bool state) {
                    _paused = state;
                    emit pausedStateChanged(state);
                }
                 
                /**
                 * Set the amplitude percentage
                 * @param amplitude value in (0, 100]
                 */
                void setAmplitude(Uint amplitude) {
                    _oscillator.setAmplitude(amplitude/100.0 * ews::physics::MAX_AMPLITUDE);
                    emit amplitudeChanged(amplitude);
                }
                
                /**
                 * Set the frequency in of drops in millihertz
                 */
                void setFrequency(Uint frequency) {
//                    _oscillator.setPeriod(1000.0/frequency);
                    _frequency = frequency;
                    emit frequencyChanged(frequency);
                }
                
                /**
                 * Set the XY position of the drip source in water medium
                 * coordinates.
                 */
                void setPosition(const osg::Vec2& pos) {
                    _oscillator.setLocation(static_cast<Uint>(pos.x()),
                                            static_cast<Uint>(pos.y()));
                    emit positionChanged(pos);
                }
                
                /**
                 * Introduce a single drop into the system.
                 */
                void pulseDrip() {
                    emit drip(_oscillator.getAmplitude());
                }
                
            private slots:
                void updateTimer();
                void pokeOscillator();
                
            signals:
                void enabledChanged(bool);
                void pausedStateChanged(bool);
                void frequencyChanged(int);
                void amplitudeChanged(int);
                void positionChanged(osg::Vec2);
                
                
                /**
                 * Fired with a value between (0,100] when
                 * a water drop is added to the system.
                 * @param amplitude strength value (0, 100].
                 */
                void drip(int amplitude);
                
            private:
                Q_DISABLE_COPY(DripSource)
                Oscillator _oscillator;
                /** This timer is only responsible for giving the user
                 *  feedback that the source is active. The Oscillator
                 *  takes care of its own triggering when it's updated with
                 *  the current time. */
                QTimer _timer;
                bool _paused;
                bool _enabled;
                Uint _frequency;
            };
        }
    }
}



#endif // DripSource_H
