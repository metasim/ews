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

#ifndef __LINE2D_H
#define __LINE2D_H

#include "Point2d.h"
#include "Vector2d.h"

namespace ews {
    namespace physics {
        /**
         * @ingroup Physics
         * 2-dimensional lines
         */
        class Line2d {
        public:
            /**
             * Default constructor - essentially creates a point.
             */
            Line2d(): _p1(), _p2(), _epsilon(0.0) { /* do nothing */ }
            /**
             * Constructor taking a begin and end point for the line
             * @param p1 Begin point
             * @param p2 End point
             */
            Line2d(const Point2d& p1, const Point2d& p2):
            _p1(p1), _p2(p2), _epsilon(Tuple2d::computeEpsilon(p1, p2)) {
                /* do nothing */
            }
            /**
             * Virtual destructor
             */
            virtual ~Line2d() { /* do nothing */ }
            /**
             * @return Begin point
             */
            Point2d getStart() const { return _p1; }
            /**
             * Sets a new starting point
             * @param p New starting point
             */
            void setStart(Point2d p) { _p1 = p; }
            /**
             * @return End point
             */
            Point2d getEnd() const { return _p2; }
            /**
             * Sets a new ending point
             * @param p New ending point
             */
            void setEnd(Point2d p) { _p2 = p; }
            /**
             * Return a scale-invariant epsilon value (i.e., one that varies with the length of the
             * line).
             * @return scale-invariant epsilon
             */
            inline double epislon() const { return _epsilon; }
            /**
             * Compute the alpha value [0, 1] (if on the line) of the given point by projecting it
             * on to the line and determining how far along the line segment it lies.
             * @return alpha value
             */
            inline double alpha(const Point2d& p) const {
                const Vector2d dir(_p2 - _p1);
                const Vector2d tmp(p - _p1);
                const double sqlen = dir.lengthSq();
                // If this is effectively a point (_p1 and _p2 are at the same location), then
                // return 0 if p is also at this location, or NaN otherwise
                return sqlen > 0 || p.x() != _p1.x() || p.y() != _p1.y() ? tmp.dot(dir) / sqlen : 0.0;
            }
            inline double length() const {
                return Vector2d(_p2 - _p1).length();
            }
            /**
             * Find an interpolated point alpha along this line segment. Return false if interpolated
             * point lies outside line segment (i.e., alpha < 0 or alpha > 1).
             */
            inline bool interpolate(double alphaVal, Point2d& interpolatedPt) const {
                interpolatedPt = Point2d(_p1, _p2, alphaVal);
                return (alphaVal >= 0.0 - _epsilon && alphaVal <= 1.0 + _epsilon);
            }
            /**
             * Project the given point on to the line segment. If the projected point falls outside
             * the segment, then return false.
             * @param ptToProject Point to project on to the line segment
             * @param projectedPt Result of projection
             * @return Wther projected point falls on the line segment
             */
            inline bool ptSegProjection(const Point2d& ptToProject, Point2d& projectedPt) const {
                const double alphaVal = alpha(ptToProject);
                return interpolate(alphaVal, projectedPt);
            }
        private:
            Line2d(const Line2d&) {} // Not allowed
            Line2d& operator=(const Line2d& l) { return *this; } // Not allowed
            Point2d _p1, _p2;
            double _epsilon;
        };
    }
}

#endif // __LINE2D_H
