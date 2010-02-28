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

#ifndef __WAVEPROPAGATOR_H
#define __WAVEPROPAGATOR_H

#include "Potential.h"
#include "Lattice.h"
#include "counted_ptr.h"
using ews::util::counted_ptr;

namespace ews {
    namespace physics {
        /**
         * @ingroup Physics
         * Interface for models describing wave propagation.
         */        
        class WavePropagator {
        public:
            virtual ~WavePropagator() { /* do nothing */ }
            virtual void setBoundaryCondition(unsigned int x, unsigned int y, double value) = 0;
            virtual void propagate(Lattice& lattice) = 0;
            virtual void scale(double scale) = 0;
            virtual void setSize(unsigned int width, unsigned int length) = 0;
            virtual const counted_ptr<const Potential>& getPotential() const = 0;
            virtual void setPotential(const counted_ptr<const Potential>& p) = 0;
            virtual void clear() = 0;
        protected:
            WavePropagator() { /* do nothing */ }
        };
    }
}

#endif // __WAVEPROPAGATOR_H
