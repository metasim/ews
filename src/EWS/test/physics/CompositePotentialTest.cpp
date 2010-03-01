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

#include "CompositePotentialTest.h"
#include "ConstantPotential.h"

namespace ews {
    namespace test {
        void CompositePotentialTest::initTestCase() { /* do nothing */ }
        void CompositePotentialTest::cleanupTestCase() { /* do nothing */ }
        void CompositePotentialTest::init() { /* do nothing */ }
        void CompositePotentialTest::cleanup() { /* do nothing */ }
        
        void CompositePotentialTest::DefaultConstructorIsZero() {
            CompositePotential p;
            // Tests that the potential is zero (unsigned int values shouldn't matter)
            QCOMPARE(p.getPotential(2, 2), 0.0);
        }
        
        void CompositePotentialTest::CanHandleSingleConstantPotential() {
            CompositePotential p;
            const double value = 3.14159;
            counted_ptr<const Potential> constP = counted_ptr<const Potential>(new ConstantPotential(value));
            p.addPotential(constP);
            // Tests that the potential is what we assigned it (unsigned int values shouldn't matter)
            QCOMPARE(p.getPotential(2, 2), value);
        }
    }
}

using ews::test::CompositePotentialTest;
QTEST_APPLESS_MAIN(CompositePotentialTest)
