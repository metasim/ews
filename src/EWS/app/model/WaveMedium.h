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

namespace ews {
    namespace app {
        namespace model {
            class WaveMedium : public QObject {
                Q_OBJECT
                Q_PROPERTY(int width READ width WRITE setWidth)
                Q_PROPERTY(int length READ length WRITE setLength)
                Q_PROPERTY(int latticeDivisionsPerCentimeter READ latticeDivisionsPerCentimeter WRITE setLatticeDivisionsPerCentimeter)
                
            public:
                /**
                 * Standard ctor.
                 */
                WaveMedium(QObject * parent = 0) : 
                QObject(parent), _width(100), _length(100), 
                _latticeDivisionsPerCentimeter(1) {

                }
                
                virtual ~WaveMedium() {}
                
                /**
                 * Get the width in centimeters.
                 * @return width in centimeters.
                 */
                int width() const {
                    return _width;
                }
                
                /**
                 * Set the width in centimeters
                 * @param width width in centimeters
                 */
                void setWidth(int width) {
                    _width = width;
                    emit sizeChanged(this->width(), this->length());
                }
                
                /**
                 * Get the length in centimeters.
                 * @return length in centimeters
                 */
                int length() const {
                    return _length;
                }
                
                /**
                 * Set the length in centimeters. 
                 */
                void setLength(int length) {
                    _length = length;
                    emit sizeChanged(this->width(), this->length());
                }
                
                /**
                 * Get the number of lattice divisions per centimeter.
                 * This is effectively how the resolution of the simulation is set.
                 * For each centimeter of width or length, the wave state
                 * is defined by that number of descrete values.
                 */
                int latticeDivisionsPerCentimeter() const {
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
                             
            private:
                Q_DISABLE_COPY(WaveMedium)
                int _width;
                int _length;
                int _latticeDivisionsPerCentimeter;
                
            };
            
            inline QDebug operator<<(QDebug dbg, const WaveMedium &ds) {
                dbg << ds.metaObject()->className() << "{";
                dbg << "name=" << ds.objectName() << ",";
                dbg << "width=" << ds.width() << ",";
                dbg << "length=" << ds.length() << ",";
                dbg << "latticeDivisionsPerCentimeter=" << ds.latticeDivisionsPerCentimeter() << ",";
                dbg << "}";
                return dbg;
            }
        }
    }
}



#endif // __WAVE_MEDIUM_H