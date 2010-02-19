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


class DripSource : public QObject
{
	Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled)
    Q_PROPERTY(int frequency READ frequency WRITE setFrequency)
    Q_PROPERTY(int amplitude READ amplitude WRITE setAmplitude)
    
public:
    /**
     * Standard ctor.
     */
    DripSource(QObject * parent = 0);
    
    /**
     * Determine if this source is enabled or not.
     */
    bool enabled() const {
        return _enabled;
    }

    
    /**
     * Get the drip frequency in milliseconds. 
     */
    int frequency() const {
        return _frequency;
    }

    
    /**
     * Get the amplitude.
     */
    int amplitude() const {
        return _amplitude;
    }
    
public slots:
    
    /**
     * Set the enabled state of this drip source.
     */
    void setEnabled(bool state) {
        qDebug() << objectName() << "enabled" << state;
        _enabled = state;
        emit enabledChanged(state);
    }
    /**
     * Set the amplitude in ????
     * TODO: what are the units here?
     */
    void setAmplitude(int amplitude) {
        qDebug() << objectName() << "amplitude" << amplitude;
        _amplitude = amplitude;
        emit amplitudeChanged(amplitude);
    }
    
    /**
     * Set the frequency in of drops in milliseconds
     */
    void setFrequency(int frequency) {
        qDebug() << objectName() << "frequency" << frequency;
        _frequency = frequency;
        emit frequencyChanged(frequency);
    }
    
    /**
     * Introduce a single drop into the system.
     */
    void pulseDrip() {
        qDebug() << objectName() << "pulse" << amplitude();
        emit drip(amplitude());
    }
    
signals:
    void enabledChanged(bool);
    void frequencyChanged(int);
    void amplitudeChanged(int);
    void drip(int amplitude);
    
private:
    Q_DISABLE_COPY(DripSource)
    bool _enabled;
    int _amplitude;
    int _frequency;
};

inline QDebug operator<<(QDebug dbg, const DripSource &ds) {
    dbg << ds.metaObject()->className() << "{";
    dbg << "name=" << ds.objectName() << ",";
    dbg << "enabled=" << ds.enabled() << ",";
    dbg << "frequency=" << ds.frequency() << ",";
    dbg << "amplitude=" << ds.amplitude();
    dbg << "}";
    return dbg;
}



#endif // DripSource_H
