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

#include "SlitPotential.h"
#include <algorithm>
#include <osg/Vec2d>
using std::sort;

namespace ews {
    namespace physics {
        using osg::Vec2d;
        
        SlitPotential::SlitPotential(const Vec2d& p1, const Vec2d& p2, unsigned int numSlits): 
        WallPotential(p1, p2), _slitWidth(DEFAULT_SLIT_WIDTH), _slitAlphas(numSlits, 0.0) {
            const double deltaAlpha = 1.0 / (1 + numSlits);
            double alpha = deltaAlpha;
            for (unsigned int i = 0; i < numSlits; i++, alpha += deltaAlpha) {
                _slitAlphas[i] = alpha;
            }
        }
        void SlitPotential::addSlit(double alphaVal) {
            _slitAlphas.push_back(alphaVal);
            sort(_slitAlphas.begin(), _slitAlphas.end());
        }
        Vec2d SlitPotential::getSlitLocation(unsigned int slitNumber) const {
            if (slitNumber >= _slitAlphas.size()) return getDstPoint();
            const double alphaVal = _slitAlphas[slitNumber];
            Vec2d interpolatedPoint;
            _lineSegment.interpolate(alphaVal, interpolatedPoint);
            return interpolatedPoint;
        }
        double SlitPotential::getPotential(unsigned int x, unsigned int y) const {
            const double wallPotential = WallPotential::getPotential(x, y);
            if (wallPotential == 0.0) return wallPotential;
            const double alphaVal = alpha(x, y);
            const double slitAlphaWidth = _slitWidth / length();
            for (vector<double>::const_iterator i = _slitAlphas.begin(); i != _slitAlphas.end(); i++) {
                // Slit window is *i +/- _slitAlphaWidth
                if (*i - slitAlphaWidth < alphaVal && alphaVal < *i + slitAlphaWidth) {
                    return 0.0;
                }
            }
            return wallPotential;
        }
    }
}