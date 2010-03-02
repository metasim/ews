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
            DampedClassicalWavePropagator(counted_ptr<const Potential>& p, unsigned int width, unsigned int length,
                                          unsigned int dampX, unsigned int dampY);
            virtual ~DampedClassicalWavePropagator() { /* do nothing */ }
            void setBoundaryCondition(unsigned int x, unsigned int y, double value);
            void propagate(Lattice& lattice);
            void scale(double scale);
            void setSize(unsigned int width, unsigned int length);
            const counted_ptr<const Potential>& getPotential() const { return _potential; }
            void setPotential(const counted_ptr<const Potential>& p);
            void clear();
        private:
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
