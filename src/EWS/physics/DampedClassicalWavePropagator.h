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

#ifndef __DAMPEDCLASSICALWAVEPROPAGATOR_H
#define __DAMPEDCLASSICALWAVEPROPAGATOR_H

#include "WavePropagator.h"
#include "Potential.h"
#include "Lattice.h"

namespace ews {
    namespace physics {
        /**
         * @ingroup Physics
         * Wave Propagator using a damped classical model.
         */        
        class DampedClassicalWavePropagator : public WavePropagator {
        public:
            /**
             * Constructor that creates a  wave propagator for a region of size width x length, with potential p, and
             * with x/y dampening factors of dampX/dampY
             * @param p Potential associated with this propagator
             * @param width Width of the region associate with this propagator
             * @param length Length of the region associate with this propagator
             * @param dampX Dampening factor in the x direction
             * @param dampY Dampening factor in the y direction
             */
            DampedClassicalWavePropagator(counted_ptr<const Potential>& p, unsigned int width, unsigned int length,
                                          unsigned int dampX, unsigned int dampY);
            /**
             * Virtual destructor
             */
            virtual ~DampedClassicalWavePropagator() { /* do nothing */ }
            /**
             * Adjusts the given point as if it had an immediate history of having the given value there.
             * @param x X-location to modify
             * @param y Y-location to modify
             * @param value Value to assign
             */
            void setBoundaryCondition(unsigned int x, unsigned int y, double value);
            /**
             * Update the given lattice using history stored in this propagator.
             * @param lattice Lattice to update
             */
            void propagate(Lattice& lattice);
            /**
             * Scale the histories of this propagator by the given factor.
             * @param scale Amount to scale by
             */
            void scale(double scale);
            /**
             * Adjusts the region over which this propagator is valid. If the valid differs from the existing
             * size, it resets all history to zero.
             */
            void setSize(unsigned int width, unsigned int length);
            /**
             * Returns the potential associated with this propagator.
             * @return Associated potential
             */
            const counted_ptr<const Potential>& getPotential() const { return _potential; }
            /**
             * Assigns a new potential to this propagator.
             * @param p New potential
             */
            void setPotential(const counted_ptr<const Potential>& p);
            /**
             * Resets the immediate history to zero.
             */
            void clear();
        private:
            /**
             * Subtracts the dampening offset added by DampedClassicalWavePropagator
             */
            class PaddedPotential : public Potential {
            public:
                PaddedPotential(const counted_ptr<const Potential>& p, unsigned int dampX, unsigned int dampY):
                _innerP(p), _dampX(dampX), _dampY(dampY) {
                    /* do nothing */
                }
                virtual ~PaddedPotential() { /* do nothing */ }
                double getPotential(unsigned int x, unsigned int y) const {
                    return _innerP->getPotential(x - _dampX, y - _dampY);
                }
            private:
                counted_ptr<const Potential> _innerP;
                unsigned int _dampX;
                unsigned int _dampY;
            };
            void dampHorizontal(unsigned int y, int delta);
            void dampHorizontal(unsigned int y, int delta, unsigned int numDampPts);
            void dampVertical(unsigned int x, int delta);
            void dampVertical(unsigned int x, int delta, unsigned int numDampPts);
            void dampScale();
            double getDamp(unsigned int depthInDampRegion);
            counted_ptr<const Potential> _potential;
            Lattice _largeLattice;
            Lattice _priorLattice;
            Lattice _priorPriorLattice;
            unsigned int _dampX;
            unsigned int _dampY;
        };
    }
}

#endif // __DAMPEDCLASSICALWAVEPROPAGATOR_H
