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
            Point2d getStart() { return _p1; }
            /**
             * Sets a new starting point
             * @param p New starting point
             */
            void setStart(Point2d p) { _p1 = p; }
            /**
             * @return End point
             */
            Point2d getEnd() { return _p2; }
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
            double epislon() const { return _epsilon; }
            /**
             * Compute the alpha value [0, 1] (if on the line) of the given point by projecting it
             * on to the line and determining how far along the line segment it lies.
             * @return alpha value
             */
            double alpha(Point2d p) const;
            /**
             * Find an interpolated point alpha along this line segment. Return false if interpolated
             * point lies outside line segment (i.e., alpha < 0 or alpha > 1).
             */
            bool interpolate(double alphaVal, Point2d& interpolatedPt) const;
            /**
             * Project the given point on to the line segment. If the projected point falls outside
             * the segment, then return false.
             * @param ptToProject Point to project on to the line segment
             * @param projectedPt Result of projection
             * @return Wther projected point falls on the line segment
             */
            bool ptSegProjection(const Point2d& ptToProject, Point2d& projectedPt) const;
        private:
            Point2d _p1, _p2;
            double _epsilon;
        };
    }
}

#endif // __LINE2D_H
