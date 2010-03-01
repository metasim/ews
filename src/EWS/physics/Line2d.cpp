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

#include "Line2d.h"
#include "Vector2d.h"

namespace ews {
    namespace physics {
        double Line2d::alpha(Point2d p) const {
            Vector2d dir(_p2 - _p1);
            Vector2d tmp(p - _p1);
            const double sqlen = dir.lengthSq();
            // If this is effectively a point (_p1 and _p2 are at the same location), then
            // return 0 if p is also at this location, or NaN otherwise
            return sqlen > 0 || p.x() != _p1.x() || p.y() != _p1.y() ? tmp.dot(dir) / sqlen : 0.0;
        }
        bool Line2d::interpolate(double alphaVal, Point2d& interpolatedPt) const {
            interpolatedPt = Point2d(_p1, _p2, alphaVal);
            return (alphaVal >= 0.0 - _epsilon && alphaVal <= 1.0 + _epsilon);
        }
        bool Line2d::ptSegProjection(const Point2d& ptToProject, Point2d& projectedPt) const {
            const double alphaVal = alpha(ptToProject);
            return interpolate(alphaVal, projectedPt);
        }
        double Line2d::length() const {
            return Vector2d(_p2 - _p1).length();
        }
    }
}
