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

#ifndef __WAVE_MEDIUM_H
#define __WAVE_MEDIUM_H


#include <QtCore>
#include "WaveModel.h"

namespace ews {
    namespace app {
        namespace model {
            using ews::physics::WaveModel;
            
            /**
             * Encapsulation of the state data associated with configuring
             * and updating the wave model.
             */
            class WaveMedium : public QObject {
                Q_OBJECT
                Q_PROPERTY(int width READ getWidth WRITE setWidth)
                Q_PROPERTY(int length READ getLength WRITE setLength)
                Q_PROPERTY(int latticeDivisionsPerCentimeter READ getLatticeDivisionsPerCentimeter WRITE setLatticeDivisionsPerCentimeter)
                
            public:
                /**
                 * Standard ctor.
                 */
                WaveMedium(QObject * parent = 0)  
                : QObject(parent), _width(128), _length(128), 
                _latticeDivisionsPerCentimeter(1), _waveModel(NULL) {
                    QObject::connect(this, SIGNAL(sizeChanged(int, int)), SLOT(updateWaveModel()));
                    QObject::connect(this, SIGNAL(resolutionChanged(int)), SLOT(updateWaveModel()));
                    
                    updateWaveModel();
                    
                }
                
                virtual ~WaveMedium() {}
                
                /**
                 * Get the width in centimeters.
                 * @return width in centimeters.
                 */
                int getWidth() const {
                    return _width;
                }
                
                /**
                 * Set the width in centimeters
                 * @param width width in centimeters
                 */
                void setWidth(int width) {
                    _width = width;
                    emit sizeChanged(this->getWidth(), this->getLength());
                }
                
                /**
                 * Get the length in centimeters.
                 * @return length in centimeters
                 */
                int getLength() const {
                    return _length;
                }
                
                /**
                 * Set the length in centimeters. 
                 */
                void setLength(int length) {
                    _length = length;
                    emit sizeChanged(this->getWidth(), this->getLength());
                }
                
                /**
                 * Get the number of lattice divisions per centimeter.
                 * This is effectively how the resolution of the simulation is set.
                 * For each centimeter of width or length, the wave state
                 * is defined by that number of descrete values.
                 */
                int getLatticeDivisionsPerCentimeter() const {
                    return _latticeDivisionsPerCentimeter;
                }
                
                /**
                 * Set the number of lattice divisions per centimeter.
                 * This is effectively how the resolution of the simulation is set.
                 * For each centimeter of width or length, the wave state
                 * is defined by that number of descrete values.
                 */
                void setLatticeDivisionsPerCentimeter(int divisions){
                    _latticeDivisionsPerCentimeter = divisions;
                    emit resolutionChanged(divisions);
                }
                
                /**
                 * Get a R/W reference to the wave model.
                 */
                WaveModel& getWaveModel() {
                    return *_waveModel;
                }
                
            signals:
                /**
                 * Signal fired when either the width or length changes.
                 * @param width new width in centimeters
                 * @param height new height in centimeters
                 */
                void sizeChanged(int width, int height);
                
                /**
                 * Signal fired when latticeDivisionsPerCentimeter changes.
                 * @param divisions new value
                 */
                void resolutionChanged(int divisions);
                
              
            private slots:
                void updateWaveModel();
                
            private:
                Q_DISABLE_COPY(WaveMedium)
                int _width;
                int _length;
                int _latticeDivisionsPerCentimeter;
                WaveModel* _waveModel;
                
            };
            
            inline QDebug operator<<(QDebug dbg, const WaveMedium &ds) {
                dbg << ds.metaObject()->className() << "{";
                dbg << "name=" << ds.objectName() << ",";
                dbg << "width=" << ds.getWidth() << ",";
                dbg << "length=" << ds.getLength() << ",";
                dbg << "latticeDivisionsPerCentimeter=" << ds.getLatticeDivisionsPerCentimeter() << ",";
                dbg << "}";
                return dbg;
            }
        }
    }
}



#endif // __WAVE_MEDIUM_H