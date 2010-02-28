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

#include "DampedClassicalWavePropagatorTest.h"
#include "ConstantPotential.h"

namespace ews {
    namespace test {
        
        void DampedClassicalWavePropagatorTest::initTestCase() { /* do nothing */ }
        void DampedClassicalWavePropagatorTest::cleanupTestCase() { /* do nothing */ }
        void DampedClassicalWavePropagatorTest::init() { /* do nothing */ }
        void DampedClassicalWavePropagatorTest::cleanup() { /* do nothing */ }
        void DampedClassicalWavePropagatorTest::ConstructorWorks() {
            counted_ptr<const Potential> p = counted_ptr<const Potential>(new ConstantPotential());
            DampedClassicalWavePropagator testDampedClassicalWavePropagator(p, 50, 50, 20, 20);
        }
        void DampedClassicalWavePropagatorTest::MethodPropagateWorksInPlace() {
            unsigned int width = 50;
            unsigned int length = 50;
            Lattice lattice(width, length);
            counted_ptr<const Potential> p = counted_ptr<const Potential>(new ConstantPotential());
            DampedClassicalWavePropagator testDampedClassicalWavePropagator(p, 50, 50, 20, 20);
            double expected = 1.0;
            unsigned int x = 10;
            unsigned int y = 10;
            lattice.setValue(x, y, expected);
            testDampedClassicalWavePropagator.setBoundaryCondition(x, y, expected);
            testDampedClassicalWavePropagator.propagate(lattice);
            // Using deltas because I'm trying to find a way to generalize this
            double delta = -1;
            expected += delta; // 0
            QCOMPARE(lattice.getValue(x, y), expected);
            testDampedClassicalWavePropagator.propagate(lattice);
            delta = -0.75;
            expected += delta; // -0.75
            QCOMPARE(lattice.getValue(x, y), expected);
            testDampedClassicalWavePropagator.propagate(lattice);
            delta = 0.25;
            expected += delta; // -0.5
            QCOMPARE(lattice.getValue(x, y), expected);
            testDampedClassicalWavePropagator.propagate(lattice);
            delta = 0.640625;
            expected += delta; // 0.140625
            QCOMPARE(lattice.getValue(x, y), expected);
            testDampedClassicalWavePropagator.propagate(lattice);
            delta = 0.171875;
            expected += delta; // 0.3125
            QCOMPARE(lattice.getValue(x, y), expected);
            testDampedClassicalWavePropagator.propagate(lattice);
            delta = -0.26171875;
            expected += delta; // 0.05078125
            QCOMPARE(lattice.getValue(x, y), expected);
            testDampedClassicalWavePropagator.propagate(lattice);
            delta = -0.16796875;
            expected += delta; // -0.1171875
            QCOMPARE(lattice.getValue(x, y), expected);
            testDampedClassicalWavePropagator.propagate(lattice);
            delta = 0.04351806640625;
            expected += delta; // -0.07366943359375
            QCOMPARE(lattice.getValue(x, y), expected);
        }
        void DampedClassicalWavePropagatorTest::MethodPropagateWorksAcrossSpace() {
            unsigned int width = 50;
            unsigned int length = 50;
            Lattice lattice(width, length);
            counted_ptr<const Potential> p = counted_ptr<const Potential>(new ConstantPotential());
            DampedClassicalWavePropagator testDampedClassicalWavePropagator(p, 50, 50, 20, 20);
            double expected = 1.0;
            unsigned int x = 10;
            unsigned int y = 10;
            lattice.setValue(x, y, expected);
            testDampedClassicalWavePropagator.setBoundaryCondition(x, y, expected);
            x++;
            for (; x < lattice.getWidth(); x++) {
                QCOMPARE(lattice.getValue(x, y), 0.0);
                testDampedClassicalWavePropagator.propagate(lattice);
                expected /= 4.0;
                QCOMPARE(lattice.getValue(x, y), expected);
            }
        }
    }
}

using ews::test::DampedClassicalWavePropagatorTest;
QTEST_APPLESS_MAIN(DampedClassicalWavePropagatorTest)
