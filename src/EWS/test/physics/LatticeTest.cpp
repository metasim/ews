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

#include "LatticeTest.h"

namespace ews {
    namespace test {
        
        void LatticeTest::initTestCase() { /* do nothing */ }
        void LatticeTest::cleanupTestCase() { /* do nothing */ }
        void LatticeTest::init() { /* do nothing */ }
        void LatticeTest::cleanup() { /* do nothing */ }
        
        void LatticeTest::MethodComputeAverageValueWorksForNormalCases() {
            Lattice testLattice(5, 5);
            // Tests that the average value for the range x = [0..4], y = [0..4] is zero
            QCOMPARE(testLattice.computeAverageValue(2, 2, 2), 0.0);
            testLattice.setValue(2, 2, 9.0);
            // "Averaging" range x = 2, y = 2
            QCOMPARE(testLattice.computeAverageValue(2, 2, 0), 9.0);
            // Averaging range x = [1..3], y = [1..3]
            QCOMPARE(testLattice.computeAverageValue(2, 2, 1), 1.0);
            // Averaging range x = [0..4], y = [0..4]
            QCOMPARE(testLattice.computeAverageValue(2, 2, 2), 9.0 / 25.0);
        }
        
        void LatticeTest::MethodComputeAverageValueWorksForWindowsGoingOutsideBounds() {
            Lattice testLattice(5, 5);
            // Tests that the average value for the range x = [0..2], y = [0..2] is zero
            QCOMPARE(testLattice.computeAverageValue(0, 0, 2), 0.0);
            testLattice.setValue(2, 2, 9.0);
            // Averaging range x = [0..1], y = [0..1]
            QCOMPARE(testLattice.computeAverageValue(0, 0, 1), 0.0);
            // Averaging range x = [0..2], y = [0..2]
            QCOMPARE(testLattice.computeAverageValue(0, 0, 2), 1.0);
            // Averaging range x = [0..2], y = [0..4]
            QCOMPARE(testLattice.computeAverageValue(0, 2, 2), 9.0 / 15.0);
            // Averaging range x = [0..4], y = [0..2]
            QCOMPARE(testLattice.computeAverageValue(2, 0, 2), 9.0 / 15.0);
            // Averaging range x = [0..2], y = [2..4]
            QCOMPARE(testLattice.computeAverageValue(0, 4, 2), 1.0);
            // Averaging range x = [2..4], y = [0..2]
            QCOMPARE(testLattice.computeAverageValue(4, 0, 2), 1.0);
        }
        
        void LatticeTest::MethodGetWidthWorks() {
            QCOMPARE(Lattice(5, 3).getWidth(), static_cast<unsigned int>(5));
            QCOMPARE(Lattice(0, 1).getWidth(), static_cast<unsigned int>(0));
            QCOMPARE(Lattice(1, 0).getWidth(), static_cast<unsigned int>(1));
        }
        
        void LatticeTest::MethodGetLengthWorks() {
            QCOMPARE(Lattice(5, 3).getLength(), static_cast<unsigned int>(3));
            QCOMPARE(Lattice(0, 1).getLength(), static_cast<unsigned int>(0)); // s/b zero because of implementation detail
            QCOMPARE(Lattice(1, 0).getLength(), static_cast<unsigned int>(0));
        }
        
        void LatticeTest::MethodSetSizeWorks() {
            Lattice testLattice(5, 5);
            testLattice.setValue(2, 2, 9.0);
            testLattice.setSize(10, 20);
            QCOMPARE(testLattice.getWidth(), static_cast<unsigned int>(10));
            QCOMPARE(testLattice.getLength(), static_cast<unsigned int>(20));
            QCOMPARE(testLattice.getValue(2, 2), 0.0);
            testLattice.setSize(0, 0);
        }
        
        void LatticeTest::MethodSetGetValueWorks() {
            Lattice testLattice(5, 5);
            testLattice.setValue(2, 2, 9.0);
            QCOMPARE(testLattice.getValue(2, 2), 9.0);
        }
        
        void LatticeTest::MethodGetValueYieldsZeroOutsideOfLattice() {
            Lattice testLattice(5, 5);
            QCOMPARE(testLattice.getValue(5, 5), 0.0);
            QCOMPARE(testLattice.getValue(500, 500), 0.0);
        }

        void LatticeTest::MethodScaleWorks() {
            Lattice testLattice(5, 5);
            testLattice.setValue(2, 2, 9.0);
            testLattice.scale(2.0);
            QCOMPARE(testLattice.getValue(2, 2), 18.0);
        }
        
        void LatticeTest::MethodScaleLocationWorks() {
            Lattice testLattice(5, 5);
            testLattice.setValue(2, 2, 9.0);
            testLattice.scaleLocation(2, 1, 2.0);
            QCOMPARE(testLattice.getValue(2, 2), 9.0);
            testLattice.scaleLocation(2, 2, 2.0);
            QCOMPARE(testLattice.getValue(2, 2), 18.0);
        }
        
        void LatticeTest::MethodClearWorks() {
            Lattice testLattice(5, 5);
            testLattice.setValue(2, 2, 9.0);
            testLattice.clear();
            QCOMPARE(testLattice.getValue(2, 2), 0.0);
        }
    }
}

using ews::test::LatticeTest;
QTEST_APPLESS_MAIN(LatticeTest)
