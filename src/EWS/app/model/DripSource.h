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
#include "WaveModel.h"
#include "Oscillator.h"

namespace ews {
    namespace app {
        namespace model {
            using ews::physics::WaveModel;
            using ews::physics::Oscillator;
            
            class DripSource : public QObject {
                Q_OBJECT
                Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
                Q_PROPERTY(unsigned int frequency READ getFrequency WRITE setFrequency)
                Q_PROPERTY(unsigned int amplitude READ getAmplitude WRITE setAmplitude)
                Q_PROPERTY(QPoint position READ getPosition WRITE setPosition)
                
            public:
                /**
                 * Standard ctor.
                 */
                DripSource(WaveModel& model, QObject * parent = 0);
                
                /**
                 * Determine if this source is enabled or not.
                 */
                bool isEnabled() const {
                    return _oscillator.getOscillateStatus();
                }
                
                
                /**
                 * Get the drip frequency in millihertz. 
                 */
                unsigned int getFrequency() const {
                    return (unsigned int) (1000.0/_oscillator.getPeriod());
                }
                
                
                /**
                 * Get the amplitude, (0, 100]
                 */
                unsigned int getAmplitude() const {
                    return (int) (_oscillator.getAmplitude() / ews::physics::MAX_AMPLITUDE * 100);
                }
                
                /**
                 * Get the position of the drip source in the X/Y plane.
                 * @return 2D location.
                 */
                QPoint getPosition() const {
                    return QPoint(_oscillator.x(), _oscillator.y());
                }
                
                /**
                 * Get the physics simulation peer object.
                 */
                Oscillator& getOscillator() {
                    return _oscillator;
                }
                
            public slots:
                
                /**
                 * Set the enabled state of this drip source.
                 */
                void setEnabled(bool state) {
                    _oscillator.setOscillateStatus(state);
                    emit enabledChanged(state);
                }
                /**
                 * Set the amplitude percentage
                 * @param amplitude value in (0, 100]
                 */
                void setAmplitude(unsigned int amplitude) {
                    _oscillator.setAmplitude(amplitude/100.0 * ews::physics::MAX_AMPLITUDE);
                    emit amplitudeChanged(amplitude);
                }
                
                /**
                 * Set the frequency in of drops in millihertz
                 */
                void setFrequency(unsigned int frequency) {
                    _oscillator.setPeriod(1000.0/frequency);
                    emit frequencyChanged(frequency);
                }
                
                void setPosition(const QPoint& pos) {
                    _oscillator.setLocation((unsigned int) pos.x(), (unsigned int) pos.y());
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
                
            signals:
                void enabledChanged(bool);
                void frequencyChanged(int);
                void amplitudeChanged(int);
                void positionChanged(QPoint);
                
                
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
                 *  takes care of it's own triggering when it's updated with
                 *  the current time. */
                QTimer _timer;
            };
            
            inline QDebug operator<<(QDebug dbg, const DripSource &ds) {
                dbg << ds.metaObject()->className() << "{";
                dbg << "name=" << ds.objectName() << ",";
                dbg << "enabled=" << ds.isEnabled() << ",";
                dbg << "frequency=" << ds.getFrequency() << ",";
                dbg << "amplitude=" << ds.getAmplitude();
                dbg << "}";
                return dbg;
            }
        }
    }
}



#endif // DripSource_H
