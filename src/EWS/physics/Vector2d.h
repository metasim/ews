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

#ifndef __VECTOR2D_H
#define __VECTOR2D_H

#include "Tuple2d.h"
#include <cmath>
using std::sqrt;

namespace ews {
    namespace physics {
        /**
         * @ingroup Physics
         * 2-dimensional vectors
         */
        class Vector2d : public Tuple2d {
        public:
            /**
             * Default constructor for point at origin
             */
            Vector2d(): Tuple2d() { /* do nothing */ }
            /**
             * Copy constructor from any Tuple2d
             * @param t Tuple2d to copy
             */
            Vector2d(const Tuple2d& t): Tuple2d(t) { /* do nothing */ }
            /**
             * Interpolating constructor
             * @param t1 Starting tuple
             * @param t2 Ending tuple
             * @param alpha Amount to interpolate
             */
            Vector2d(const Tuple2d& t1, const Tuple2d& t2, double alpha):
            Tuple2d(t1, t2, alpha) { /* do nothing */ }
            /**
             * Constructor assigning location of x, y
             */
            Vector2d(double x, double y): Tuple2d(x, y) { /* do nothing */ }
            /**
             * Virtual destructor
             */
            virtual ~Vector2d() { /* do nothing */ }
            /**
             * Get the length squared of the vector (faster than getting the length)
             * @return length squared
             */
            double lengthSq() const {
                return x() * x() + y() * y();
            }
            /**
             * Get the length of the vector
             * @return length
             */
            double length() const {
                return sqrt(lengthSq());
            }
            /**
             * Get the dot product of this with v
             * @param v Vector to get dot product with
             * @return dot product
             */
            double dot(const Vector2d& v) const {
                return x() * v.x() + y() * v.y();
            }
        };
    }
}

#endif // __VECTOR2D_H
