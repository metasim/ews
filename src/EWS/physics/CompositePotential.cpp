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

#include "CompositePotential.h"

#include <algorithm>
using std::find;

namespace ews {
    namespace physics {
        double CompositePotential::getPotential(unsigned int x, unsigned int y) const {
            double retval = 0.0;
            for (vector<counted_ptr<const Potential> >::const_iterator i = _potentials.begin();
                 i != _potentials.end(); i++) {
                retval += (*i)->getPotential(x, y);
            }
            return retval;
        }
        void CompositePotential::addPotential(counted_ptr<const Potential>& p) {
            _potentials.push_back(p);
        }
        void CompositePotential::removePotential(counted_ptr<const Potential>& p) {
            vector<counted_ptr<const Potential> >::iterator i = find(_potentials.begin(),
                                                                     _potentials.end(), p);
            if (i != _potentials.end()) { // If found, remove
                _potentials.erase(i);
            }
        }
    }
}
