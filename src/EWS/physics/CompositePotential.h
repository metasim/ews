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

#ifndef __COMPOSITEPOTENTIAL_H
#define __COMPOSITEPOTENTIAL_H

#include "Potential.h"
#include <vector>
using std::vector;
#include "util/counted_ptr.h"
using ews::util::counted_ptr;

namespace ews {
    namespace physics {
        /**
         * @ingroup Physics
         * Potential made up of the sum of other potentials.
         */        
        class CompositePotential : public Potential {
        public:
            CompositePotential() { /* do nothing */ }
            virtual ~CompositePotential() { /* do nothing */ }
            double getPotential(unsigned int x, unsigned int y, unsigned int time) const;
            void addPotential(counted_ptr<const Potential>& p);
            void removePotential(counted_ptr<const Potential>& p);
        private:
            vector<counted_ptr<const Potential> > _potentials;
        };
    }
}

#endif // __COMPOSITEPOTENTIAL_H