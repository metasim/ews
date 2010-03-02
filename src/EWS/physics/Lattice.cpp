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

#include "Lattice.h"
#include <algorithm>
using std::max;
using std::min;

namespace ews {
    namespace physics {
        double Lattice::computeAverageValue(unsigned int x, unsigned int y, unsigned int windowWidth) const {
            double sum = 0.0;
            if (getSize() == 0) return sum;
            const unsigned int minX = max(static_cast<int>(x) - static_cast<int>(windowWidth), 0);
            const unsigned int maxX = min(x + windowWidth, static_cast<unsigned int>(getWidth()) - 1);
            const unsigned int minY = max(static_cast<int>(y) - static_cast<int>(windowWidth), 0);
            const unsigned int maxY = min(y + windowWidth, static_cast<unsigned int>(getLength()) - 1);
            const unsigned int count = (maxX - minX + 1) * (maxY - minY + 1);
            for (unsigned int i = minX; i <= maxX; i++) {
                for (unsigned int j = minY; j <= maxY; j++) {
                    sum += getValue(i, j);
                }
            }
            return sum / count;    
        }
        void Lattice::scale(double scaleVal) {
            if (scaleVal == 1.0) return;

            unsigned int size = getSize();
            for(unsigned int i = 0; i < size; i++) {
                _amplitudeData[i] *= scaleVal;
            }
        }
    }
}