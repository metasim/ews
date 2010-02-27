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

#include "WaveModel.h"
#include "DampedClassicalWavePropagator.h"
#include "ConstantPotential.h"

namespace ews {
    namespace physics {
        WaveModel::WaveModel(unsigned int width, unsigned length): _lattice(width, length),
        _wavePropagator(counted_ptr<WavePropagator>(createDefaultPropagator(width, length, 20, 20))) {
            /* do nothing */
        }
        WaveModel::WaveModel(unsigned int width, unsigned length, unsigned int dampX, unsigned int dampY):
        _lattice(width, length),
        _wavePropagator(counted_ptr<WavePropagator>(createDefaultPropagator(width, length, dampX, dampY))) {
            /* do nothing */
        }
        WavePropagator* WaveModel::createDefaultPropagator(unsigned int width, unsigned int length,
                                                       unsigned int dampX, unsigned int dampY) {
            counted_ptr<const Potential> p(new ConstantPotential());
            return new DampedClassicalWavePropagator(p, width, length, dampX, dampY);
        }
    }
}