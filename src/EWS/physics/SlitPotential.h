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

#ifndef __SLITPOTENTIAL_H
#define __SLITPOTENTIAL_H

#include "WallPotential.h"
#include "Point2d.h"
#include <vector>
using std::vector;

namespace ews {
    namespace physics {
        const double DEFAULT_SLIT_WIDTH = 2;
        /**
         * @ingroup Physics
         * Potential with a constant value everywhere.
         */        
        class SlitPotential : public WallPotential {
        public:
            /**
             * Constructor taking number of slits
             * @param numSlits how many slits to create
             */
            SlitPotential(const Point2d& p1, const Point2d& p2, unsigned int numSlits = 0);
            /**
             * Virtual destructor
             */
            virtual ~SlitPotential() { /* do nothing */}
            /**
             * Add a slit at a specified fraction of the distance along its wall
             * @param alphaVal fraction of the distance along the wall to add the slit
             */
            void addSlit(double alphaVal);
            /**
             * Remove the nth slit
             * @param slitNumber Which slit to delete (slits are sorted by location)
             */
            void removeSlit(unsigned int slitNumber) {
                if (slitNumber < _slitAlphas.size()) {
                    _slitAlphas.erase(_slitAlphas.begin() + slitNumber);
                }
            }
            Point2d getSlitLocation(unsigned int slitNumber) const;
            /**
             * Get the width of the slits
             */
            double getSlitWidth() const { return _slitWidth; }
            /**
             * Set the width of the slits
             * @param slitWidth New slit width (values less than zero will use zero instead)
             */
            void setSlitWidth(double slitWidth) { _slitWidth = slitWidth > 0 ? slitWidth : 0.0; }
            /**
             * Returns the potential at x, y
             * @param x X location to check
             * @param y Y location to check
             * @return Potential
             */
            virtual double getPotential(unsigned int x, unsigned int y) const;
        private:
            double _slitWidth;
            // Fraction that each slit is along the wall
            vector<double> _slitAlphas;
        };
    }
}

#endif // __SLITPOTENTIAL_H