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

#ifndef __LATTICE_H
#define __LATTICE_H

#include <vector>
using std::vector;

namespace ews {
    namespace physics {
        class Lattice {
            
        public:
            /**
             * Creates a lattice of size width x length, initialized to 0
             * @param width Width of the lattice
             * @param length Length of the lattice
             */
            Lattice(unsigned int width, unsigned int length): _amplitude(width, vector<double>(length, 0.0)) {
            }
            /**
             * Destructor
             */
            virtual ~Lattice() {
            }
            /**
             * Computes the average value of the lattice at x, y with an averaging window of width windowWidth.
             * @param x X location to compute the average for
             * @param y Y location to compute the average for
             * @param windoWidth Width of the averaging window (0 is a single cell)
             * @return Average value for the specified window.
             */
            double computeAverageValue(unsigned int x, unsigned int y, unsigned int windowWidth);
            /**
             * Gets the width of the lattice.
             * @return Width of the lattice
             */
            unsigned int getWidth() {
                return _amplitude.size();
            }
            /**
             * Gets the length of the lattice.
             * @return Length of the lattice
             */
            unsigned int getLength() {
                return _amplitude.empty() ? 0 : _amplitude[0].size();
            }
            /**
             * Sets the width and length of the lattice, and resets all amplitudes to zero.
             * @param width Width of the lattice
             * @param length Length of the lattice
             */
            void setSize(unsigned int width, unsigned int length) {
                _amplitude = vector<vector<double> >(width, vector<double>(length, 0.0));
            }
            /**
             * Gets the amplitude of the lattice at location x,y
             * @param x X location to get the amplitude for
             * @param y Y location to get the amplitude for
             */
            double getValue(unsigned int x, unsigned int y) {
                return _amplitude.at(x).at(y);
            }
            /**
             * Sets the amplitude of the lattice at location x,y to value
             * @param x X location to set the amplitude at
             * @param y Y location to set the amplitude at
             * @param value New value of the amplitude
             */
            void setValue(unsigned int x, unsigned int y, double value) {
                _amplitude.at(x).at(y) = value;
            }
            
        private:
            vector<vector<double> > _amplitude;
        };
    }
}

#endif // __LATTICE_H