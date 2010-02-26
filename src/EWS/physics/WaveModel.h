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

#ifndef __WAVEMODEL_H
#define __WAVEMODEL_H

#include "Lattice.h"
#include "WavePropagator.h"
#include "counted_ptr.h"
using ews::util::counted_ptr;

namespace ews {
    namespace physics {
        class WaveModel {
        public:
            WaveModel(unsigned int width, unsigned length);
            WaveModel(unsigned int width, unsigned length, unsigned int dampX, unsigned int dampY);
            virtual ~WaveModel() { /* do nothing */ }
            void propagate() { _wavePropagator->propagate(_lattice); }
            void setSize(unsigned int width, unsigned int length) {
                _lattice.setSize(width, length);
                _wavePropagator->setSize(width, length);
            }
            bool containsLocation(unsigned int x, unsigned int y) const {
                return _lattice.containsLocation(x, y);
            }
            double getValue(unsigned int x, unsigned int y) const { return _lattice.getValue(x, y); }
            double computeAverageValue(unsigned int x, unsigned int y, unsigned int windowWidth) const {
                return _lattice.computeAverageValue(x, y, windowWidth);
            }
            void setSourceValue(unsigned int x, unsigned int y, double value) {
                _lattice.setValue(x, y, value);
                _wavePropagator->setBoundaryCondition(x, y, value);
            }
            unsigned int getWidth() const { return _lattice.getWidth(); }
            unsigned int getLength() const { return _lattice.getLength(); }
            const counted_ptr<const Potential>& getPotential() const {
                return _wavePropagator->getPotential();
            }
            void setPotential(counted_ptr<const Potential> p) { _wavePropagator->setPotential(p); }
            void clear() {
                _lattice.clear();
                _wavePropagator->clear();
            }
        private:
            static WavePropagator* createDefaultPropagator(unsigned int width, unsigned int length,
                                                           unsigned int dampX, unsigned int dampY);
            Lattice _lattice;
            counted_ptr<WavePropagator> _wavePropagator;
        };
    }
}


#endif // __WAVEMODEL_H
