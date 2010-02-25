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

#ifndef __WALLPOTENTIAL_H
#define __WALLPOTENTIAL_H

#include "Potential.h"
#include "Line2d.h"

#include <cmath>
using std::sqrt;

namespace ews {
    namespace physics {
        const unsigned int DEFAULT_THICKNESS_SQ = 3 * 3;
        const double DEFAULT_POTENTIAL = 100.0;

        class WallPotential : public Potential {
        public:
            /**
             * Default constructor, with both points at origin (0, 0)
             */
            WallPotential():
            _lineSegment(Point2d(0.0, 0.0), Point2d(0.0, 0.0)), _thicknessSq(DEFAULT_THICKNESS_SQ) {
                /* do nothing */
            }
            /**
             * Constructor for a wall with end points p1, p2 and thickness 3.
             * @param p1 One point of the wall
             * @param p2 The other point of the wall
             */
            WallPotential(Point2d p1, Point2d p2):
            _lineSegment(p1, p2), _thicknessSq(DEFAULT_THICKNESS_SQ) {
                /* do nothing */
            }
            /**
             * Virtual destructor
             */
            virtual ~WallPotential() { /* do nothing */}
            /**
             * Returns a potential of 100 if and only if x, y lie within the wall.
             * @param x X location to check for wall
             * @param y Y location to check for wall
             * @param time ignored
             * @return 100 if x, y lie within wall, 0 otherwise
             */
            virtual double getPotential(unsigned int x, unsigned int y, unsigned int time);
            /**
             * Returns one of the wall's end points
             * @Return wall end point
             */
            Point2d getSrcPoint() { return _lineSegment.getStart(); }
            /**
             * Changes one of the wall's end points
             * @param p New end point to use
             */
            void setSrcPoint(Point2d p) {
                _lineSegment.setStart(p);
            }
            /**
             * Returns the wall's other end point
             * @Return other wall end point
             */
            Point2d getDstPoint() { return _lineSegment.getEnd(); }
            /**
             * Changes the wall's other end point
             * @param p New end point to use
             */
            void setDstPoint(Point2d p) {
                _lineSegment.setEnd(p);
            }
            /**
             * Returns the thickness of the wall.
             * @return Wall thickness
             */
            unsigned int getThickness() { return static_cast<unsigned int>(sqrt(_thicknessSq)); }
            /**
             * Sets the thickness of the wall.
             * @param thickness New thickness for the wall.
             */
            void setThickness(unsigned int thickness) { _thicknessSq = thickness * thickness; }
        private:
            Line2d _lineSegment;
            unsigned int _thicknessSq;
        };
    }
}

#endif // __WALLPOTENTIAL_H
