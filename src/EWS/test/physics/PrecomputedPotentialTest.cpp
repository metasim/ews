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

#include "PrecomputedPotentialTest.h"
#include "ConstantPotential.h"

namespace ews {
    namespace test {
        void PrecomputedPotentialTest::initTestCase() { /* do nothing */ }
        void PrecomputedPotentialTest::cleanupTestCase() { /* do nothing */ }
        void PrecomputedPotentialTest::init() { /* do nothing */ }
        void PrecomputedPotentialTest::cleanup() { /* do nothing */ }
        
        void PrecomputedPotentialTest::DefaultConstructorIsZero() {
            PrecomputedPotential p;
            // Tests that the potential is zero (unsigned int values shouldn't matter)
            QCOMPARE(p.getPotential(2, 2), 0.0);
        }
        
        void PrecomputedPotentialTest::CanHandleSingleConstantPotential() {
            const double value = 3.14159;
            counted_ptr<const Potential> constP = counted_ptr<const Potential>(new ConstantPotential(value));
            PrecomputedPotential p(constP, 10, 10);
            QCOMPARE(p.getPotential(2, 2), value);
            QCOMPARE(p.getPotential(10, 2), 0.0);
            QCOMPARE(p.getPotential(2, 10), 0.0);
        }
    }
}

using ews::test::PrecomputedPotentialTest;
QTEST_APPLESS_MAIN(PrecomputedPotentialTest)
