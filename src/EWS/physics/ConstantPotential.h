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

#ifndef __CONSTANTPOTENTIAL_H
#define __CONSTANTPOTENTIAL_H

#include "Potential.h"

namespace ews {
    namespace physics {
        class ConstantPotential : public Potential {
        public:
            ConstantPotential(): _value(0.0) { /* do nothing */ }
            ConstantPotential(double value): _value(value) { /* do nothing */ }
            virtual double getPotential(unsigned int x, unsigned int y, unsigned int time) {
                return _value;
            }
        private:
            double _value;
        };
    }
}

#endif // __CONSTANTPOTENTIAL_H