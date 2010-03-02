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

#ifndef __POINT2D_H
#define __POINT2D_H

#include "Tuple2d.h"

namespace ews {
    namespace physics {
        /**
         * @ingroup Physics
         * 2-dimensional points
         */
        class Point2d : public Tuple2d {
        public:
            /**
             * Default constructor for point at origin
             */
            Point2d(): Tuple2d() { /* do nothing */ }
            /**
             * Copy constructor from any Tuple2d
             * @param t Tuple2d to copy
             */
            Point2d(const Tuple2d& t): Tuple2d(t) { /* do nothing */ }
            /**
             * Interpolating constructor
             * @param t1 Starting tuple
             * @param t2 Ending tuple
             * @param alpha Amount to interpolate
             */
            Point2d(const Tuple2d& t1, const Tuple2d& t2, double alpha):
            Tuple2d(t1, t2, alpha) { /* do nothing */ }
            /**
             * Equality operator
             */
            bool operator==(const Point2d& p) const { return sameLocation(p); }
            /**
             * Constructor assigning location of x, y
             */
            Point2d(double x, double y): Tuple2d(x, y) { /* do nothing */ }
            /**
             * Virtual destructor
             */
            virtual ~Point2d() { /* do nothing */ }
        };
    }
}

#endif // __POINT2D_H
