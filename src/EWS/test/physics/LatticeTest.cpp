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

void LatticeTest::initTestCase() { /* do nothing */ }
void LatticeTest::cleanupTestCase() { /* do nothing */ }
void LatticeTest::init() { /* do nothing */ }
void LatticeTest::cleanup() { /* do nothing */ }

void LatticeTest::MethodComputeAverageValueWorksForNormalCases() {
    Lattice testLattice(5, 5);
    // Tests that the average value for the range x = [0..4], y = [0..4] is zero
    QCOMPARE(static_cast<double>(0.0), testLattice.computeAverageValue(2, 2, 2));
    testLattice.setValue(2, 2, 9.0);
    // "Averaging" range x = 2, y = 2
    QCOMPARE(static_cast<double>(9.0), testLattice.computeAverageValue(2, 2, 0));
    // Averaging range x = [1..3], y = [1..3]
    QCOMPARE(static_cast<double>(1.0), testLattice.computeAverageValue(2, 2, 1));
    // Averaging range x = [0..4], y = [0..4]
    QCOMPARE(static_cast<double>(9.0/25.0), testLattice.computeAverageValue(2, 2, 2));
}

void LatticeTest::MethodComputeAverageValueWorksForWindowsGoingOutsideBounds() {
    Lattice testLattice(5, 5);
    // Tests that the average value for the range x = [0..2], y = [0..2] is zero
    QCOMPARE(static_cast<double>(0.0), testLattice.computeAverageValue(0, 0, 2));
    testLattice.setValue(2, 2, 9.0);
    // Averaging range x = [0..1], y = [0..1]
    QCOMPARE(static_cast<double>(0.0), testLattice.computeAverageValue(0, 0, 1));
    // Averaging range x = [0..2], y = [0..2]
    QCOMPARE(1.0, testLattice.computeAverageValue(0, 0, 2));
    // Averaging range x = [0..2], y = [0..4]
    QCOMPARE(static_cast<double>(9.0/15.0), testLattice.computeAverageValue(0, 2, 2));
    // Averaging range x = [0..4], y = [0..2]
    QCOMPARE(static_cast<double>(9.0/15.0), testLattice.computeAverageValue(2, 0, 2));
    // Averaging range x = [0..2], y = [2..4]
    QCOMPARE(static_cast<double>(1.0), testLattice.computeAverageValue(0, 4, 2));
    // Averaging range x = [2..4], y = [0..2]
    QCOMPARE(static_cast<double>(1.0), testLattice.computeAverageValue(4, 0, 2));
}

void LatticeTest::MethodGetWidthWorks() {
    QCOMPARE(static_cast<unsigned int>(5), Lattice(5, 3).getWidth());
    QCOMPARE(static_cast<unsigned int>(0), Lattice(0, 1).getWidth());
    QCOMPARE(static_cast<unsigned int>(1), Lattice(1, 0).getWidth());
}

void LatticeTest::MethodGetLengthWorks() {
    QCOMPARE(static_cast<unsigned int>(3), Lattice(5, 3).getLength());
    QCOMPARE(static_cast<unsigned int>(0), Lattice(0, 1).getLength()); // s/b zero because of implementation detail
    QCOMPARE(static_cast<unsigned int>(0), Lattice(1, 0).getLength());
}

void LatticeTest::MethodSetSizeWorks() {
    Lattice testLattice(5, 5);
    testLattice.setValue(2, 2, 9.0);
    testLattice.setSize(10, 20);
    QCOMPARE(static_cast<unsigned int>(10), testLattice.getWidth());
    QCOMPARE(static_cast<unsigned int>(20), testLattice.getLength());
    QCOMPARE(static_cast<double>(0.0), testLattice.getValue(2, 2));
    testLattice.setSize(0, 0);
}

void LatticeTest::MethodSetGetValueWorks() {
    Lattice testLattice(5, 5);
    testLattice.setValue(2, 2, 9.0);
    QCOMPARE(static_cast<double>(9.0), testLattice.getValue(2, 2));
}

QTEST_MAIN(LatticeTest)
//#include "LatticeTest.moc"
