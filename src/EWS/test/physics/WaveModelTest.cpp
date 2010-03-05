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

#include "WaveModelTest.h"

namespace ews {
    namespace test {
        
        void WaveModelTest::initTestCase() { /* do nothing */ }
        void WaveModelTest::cleanupTestCase() { /* do nothing */ }
        void WaveModelTest::init() { /* do nothing */ }
        void WaveModelTest::cleanup() { /* do nothing */ }
        void WaveModelTest::ConstructorWorks() {
            WaveModel testWaveModel(50, 50);
            // Tests that the average value for the range x = [0..4], y = [0..4] is zero
            QCOMPARE(testWaveModel.computeAverageValue(2, 2, 2), 0.0);
        }
        void WaveModelTest::MethodPropagateWorksInPlace() {
            WaveModel testWaveModel(50, 50);
            double expected = 1.0;
            unsigned int x = 10;
            unsigned int y = 10;
            testWaveModel.setSourceValue(10, 10, expected);
            QCOMPARE(testWaveModel.getValue(x, y), expected);
            testWaveModel.propagate();
            double prior = expected;
            double priorPrior = prior;
            const double w1 = 0.14;
            const double w2 = 0.06;
            const double w3 = 1.1;
            const double w4 = -0.95;
            expected = w3 * prior + w4 * priorPrior;
            QCOMPARE(testWaveModel.getValue(x, y), expected);
            double neighbor = w1 * prior;
            QCOMPARE(testWaveModel.getValue(x, y + 1), neighbor);
            double diagNeigh = w2 * prior;
            QCOMPARE(testWaveModel.getValue(x + 1, y + 1), diagNeigh);
            testWaveModel.propagate();
            priorPrior = prior;
            prior = expected;
            expected = 4 * (neighbor * w1 + diagNeigh * w2) + w3 * prior + w4 * priorPrior;            
            QCOMPARE(testWaveModel.getValue(x, y), expected);
            double priorNeigh = neighbor;
            neighbor = (prior + 2 * diagNeigh) * w1 + priorNeigh * (2 * w2 + w3);
            QCOMPARE(testWaveModel.getValue(x, y + 1), neighbor);
            diagNeigh = 2 * priorNeigh * w1 + prior * w2 + diagNeigh * w3;
            QCOMPARE(testWaveModel.getValue(x + 1, y + 1), diagNeigh);
            testWaveModel.propagate();
            priorPrior = prior;
            prior = expected;
            expected = 4 * (neighbor * w1 + diagNeigh * w2) + w3 * prior + w4 * priorPrior;            
            QCOMPARE(testWaveModel.getValue(x, y), expected);
        }
        void WaveModelTest::MethodPropagateWorksAcrossSpace() {
            WaveModel testWaveModel(50, 50);
            double expected = 1.0;
            unsigned int x = 10;
            unsigned int y = 10;
            testWaveModel.setSourceValue(x, y, expected);
            QCOMPARE(testWaveModel.getValue(x, y), expected);
            x++;
            const double w1 = 0.14;
            const double w2 = 0.06;
            QBENCHMARK {
                for (; x < testWaveModel.getWidth(); x++) {
                    QCOMPARE(testWaveModel.getValue(x, y), 0.0);
                    QCOMPARE(testWaveModel.getValue(x, y - 1), 0.0);
                    QCOMPARE(testWaveModel.getValue(x, y + 1), 0.0);
                    const double priorDiag = testWaveModel.getValue(x - 1, y + 1);
                    testWaveModel.propagate();
                    expected *= w1;
                    expected += 2 * priorDiag * w2; // From both diagonals
                    QCOMPARE(testWaveModel.getValue(x, y), expected);
                }
            }
        }
    }
}

using ews::test::WaveModelTest;
QTEST_APPLESS_MAIN(WaveModelTest)
