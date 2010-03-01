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

#ifndef __PRECOMUPTEDPOTENTIAL_H
#define __PRECOMUPTEDPOTENTIAL_H

#include "Potential.h"
#include "counted_ptr.h"
using ews::util::counted_ptr;
#include <vector>
using std::vector;

namespace ews {
    namespace physics {
        /**
         * @ingroup Physics
         * Potential that pre-calculates the values of another potential for all valid spatial locations.
         */        
        class PrecomputedPotential : public Potential {
        public:
            /**
             * Default constructor
             */
            PrecomputedPotential(): _potentialValues() { /* do nothing */ }
            /**
             * Constructor that actually calculates a potential
             * @param p Potential to precompute
             * @param width Width of region where potential is valid
             * @param length Length of region where potential is valid
             */
            PrecomputedPotential(counted_ptr<const Potential> p, unsigned int width, unsigned int length);
            /**
             * Virtual destructor
             */
            virtual ~PrecomputedPotential() { /* do nothing */}
            /**
             * Returns the constant potential, regardless of x, y, or time
             * @param x ignored
             * @param y ignored
             * @param time ignored
             * @return Precomputed potential
             */
            double getPotential(unsigned int x, unsigned int y, unsigned int time) const {
                return (0 <= x) && (x < getWidth()) && (0 <= y) && (y < getLength()) ? _potentialValues[x][y] : 0.0;
            }
        private:
            unsigned int getWidth() const { return _potentialValues.size(); }
            unsigned int getLength() const {
                return _potentialValues.size() > 0 ? _potentialValues[0].size() : 0;
            }
            vector<vector<double> > _potentialValues;
        };
    }
}

#endif // __PRECOMUPTEDPOTENTIAL_H