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

#include "WallPotential.h"
#include "Vector2d.h"

namespace ews {
    namespace physics {
        double WallPotential::getPotential(unsigned int x, unsigned int y) const {
            Point2d pointToProject(static_cast<double>(x), static_cast<double>(y));
            Point2d projectedPoint;            
            if (!_lineSegment.ptSegProjection(pointToProject, projectedPoint)) return 0.0;
            return Vector2d(pointToProject - projectedPoint).lengthSq() < _thicknessSq ? DEFAULT_POTENTIAL : 0.0;
        }
        double WallPotential::alpha(unsigned int x, unsigned int y) const {
            Point2d pointToProject(static_cast<double>(x), static_cast<double>(y));
            return _lineSegment.alpha(pointToProject);
        }
    }
}