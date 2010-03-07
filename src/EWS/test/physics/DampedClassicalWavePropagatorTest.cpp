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
            DampedClassicalWavePropagator testDampedClassicalWavePropagator(p, width, length, 20, 20);
            LatticeVal expected = 1.0;
            unsigned int x = 10;
            unsigned int y = 10;
            lattice.setValue(x, y, expected);
            testDampedClassicalWavePropagator.setBoundaryCondition(x, y, expected);
            testDampedClassicalWavePropagator.propagate(lattice);
            LatticeVal prior = expected;
            LatticeVal priorPrior = prior;
            const LatticeVal w1 = 0.14;
            const LatticeVal w2 = 0.06;
            const LatticeVal w3 = 1.1;
            const LatticeVal w4 = -0.95;
            expected = w3 * prior + w4 * priorPrior;
            QCOMPARE(lattice.getValue(x, y), expected);
            LatticeVal neighbor = w1 * prior;
            QCOMPARE(lattice.getValue(x, y + 1), neighbor);
            LatticeVal diagNeigh = w2 * prior;
            QCOMPARE(lattice.getValue(x + 1, y + 1), diagNeigh);
            testDampedClassicalWavePropagator.propagate(lattice);
            priorPrior = prior;
            prior = expected;
            expected = 4 * (neighbor * w1 + diagNeigh * w2) + w3 * prior + w4 * priorPrior;            
            QCOMPARE(lattice.getValue(x, y), expected);
            double priorNeigh = neighbor;
            neighbor = (prior + 2 * diagNeigh) * w1 + priorNeigh * (2 * w2 + w3);
            QCOMPARE(lattice.getValue(x, y + 1), neighbor);
            diagNeigh = 2 * priorNeigh * w1 + prior * w2 + diagNeigh * w3;
            QCOMPARE(lattice.getValue(x + 1, y + 1), diagNeigh);
            testDampedClassicalWavePropagator.propagate(lattice);
            priorPrior = prior;
            prior = expected;
            expected = 4 * (neighbor * w1 + diagNeigh * w2) + w3 * prior + w4 * priorPrior;            
            QCOMPARE(lattice.getValue(x, y), expected);
        }
        void DampedClassicalWavePropagatorTest::MethodPropagateWorksAcrossSpace() {
            unsigned int width = 50;
            unsigned int length = 50;
            Lattice lattice(width, length);
            counted_ptr<const Potential> p = counted_ptr<const Potential>(new ConstantPotential());
            DampedClassicalWavePropagator testDampedClassicalWavePropagator(p, width, length, 20, 20);
            LatticeVal expected = 1.0;
            unsigned int x = 10;
            unsigned int y = 10;
            lattice.setValue(x, y, expected);
            testDampedClassicalWavePropagator.setBoundaryCondition(x, y, expected);
            x++;
            const LatticeVal w1 = 0.14;
            const LatticeVal w2 = 0.06;
            const LatticeVal zero = 0.0f;
            for (; x < lattice.getWidth(); x++) {
                QCOMPARE(lattice.getValue(x, y), zero);
                QCOMPARE(lattice.getValue(x, y - 1), zero);
                QCOMPARE(lattice.getValue(x, y + 1), zero);
                const LatticeVal priorDiag = lattice.getValue(x - 1, y + 1);
                testDampedClassicalWavePropagator.propagate(lattice);
                expected *= w1;
                expected += 2 * priorDiag * w2; // From both diagonals
                QCOMPARE(lattice.getValue(x, y), expected);
            }
        }
        void DampedClassicalWavePropagatorTest::BenchmarkPropagate() {
            unsigned int width = 500;
            unsigned int length = 500;
            Lattice lattice(width, length);
            counted_ptr<const Potential> p = counted_ptr<const Potential>(new ConstantPotential());
            DampedClassicalWavePropagator testDampedClassicalWavePropagator(p, width, length, 20, 20);
            LatticeVal expected = 1.0;
            unsigned int x = 10;
            unsigned int y = 10;
            lattice.setValue(x, y, expected);
            testDampedClassicalWavePropagator.setBoundaryCondition(x, y, expected);
            QBENCHMARK {
                for (; x < lattice.getWidth(); x++) {
                    testDampedClassicalWavePropagator.propagate(lattice);
                }
            }
        }
    }
}

using ews::test::DampedClassicalWavePropagatorTest;
QTEST_APPLESS_MAIN(DampedClassicalWavePropagatorTest)
