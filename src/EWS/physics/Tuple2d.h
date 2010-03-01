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

#ifndef __TUPLE2D_H
#define __TUPLE2D_H

#include <cmath>
using std::sqrt;

namespace ews {
    namespace physics {
        const double EPSILON_FACTOR = 1e-6; 
        
        /**
         * @ingroup Physics
         * Base class for 2d objects. Note: this class is not read-only since it has an
         * assignment operator.
         */
        class Tuple2d {
        public:
            /**
             * Default constructor for tuple at origin
             */
            Tuple2d(): _x(0.0), _y(0.0) { /* do nothing */ }
            /**
             * Copy constructor from any Tuple2d
             * @param t Tuple2d to copy
             */
            Tuple2d(const Tuple2d& t): _x(t._x), _y(t._y) { /* do nothing */ }
            /**
             * Interpolating constructor
             * @param t1 Starting tuple
             * @param t2 Ending tuple
             * @param alpha Amount to interpolate
             */
            Tuple2d(const Tuple2d& t1, const Tuple2d& t2, double alpha):
            _x(t1._x + alpha * (t2._x - t1._x)), _y(t1._y + alpha * (t2._y - t1._y)) {
                /* do nothing */
            }
            /**
             * Constructor assigning location of x, y
             */
            Tuple2d(double x, double y): _x(x), _y(y) { /* do nothing */ }
            /**
             * Virtual destructor
             */
            virtual ~Tuple2d() { /* do nothing */ }
            /**
             * Calculate a scale invariant epsilon, i.e., one that varies with the distance between
             * t1 and t2.
             * @param t1 First Tuple2d
             * @param t2 Second Tuple2d
             * @return scale invariant epsilon
             */
            static double computeEpsilon(const Tuple2d& t1, const Tuple2d& t2) {
                const double xDelta = (t1._x - t2._x);
                const double yDelta = (t1._y - t2._y);
                return sqrt(xDelta * xDelta + yDelta * yDelta) * EPSILON_FACTOR;
            }
            /**
             * Get the x coordinate
             * @return x coordinate
             */
            double x() const { return _x; }
            /**
             * Get the y coordinate
             * @return y coordinate
             */
            double y() const { return _y; }
            /**
             * Checks whether another tuple is at the same location as this one. Does not verify that
             * tuple is of the same type (e.g., Vector2d vs. Point2d)
             */
            bool sameLocation(const Tuple2d& o) const {
                return _x == o._x && _y == o._y;
            }
            /**
             * Assignment operator
             * @param rhs Tuple2d to copy
             * @return this
             */
            Tuple2d operator=(const Tuple2d& rhs) {
                if (this != &rhs) {
                    _x = rhs._x;
                    _y = rhs._y;
                }
                return *this;
            }
            /**
             * Subtraction operator
             * @param rhs Tuple2d to subtract from this
             * @return Difference
             */
            Tuple2d operator-(const Tuple2d& rhs) const {
                return Tuple2d(_x - rhs._x, _y - rhs._y);
            }
        private:
            double _x, _y;
        };
        
    }
}

#endif // __TUPLE2D_H
