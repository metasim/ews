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
            testWaveModel.setSourceValue(10, 10, 1);
            double expected = 1.0;
            QCOMPARE(testWaveModel.getValue(10, 10), 1.0);
            testWaveModel.propagate();
            // Using deltas because I'm trying to find a way to generalize this
            double delta = -1;
            expected += delta; // 0
            QCOMPARE(testWaveModel.getValue(10, 10), expected);
            testWaveModel.propagate();
            delta = -0.75;
            expected += delta; // -0.75
            QCOMPARE(testWaveModel.getValue(10, 10), expected);
            testWaveModel.propagate();
            delta = 0.25;
            expected += delta; // -0.5
            QCOMPARE(testWaveModel.getValue(10, 10), expected);
            testWaveModel.propagate();
            delta = 0.640625;
            expected += delta; // 0.140625
            QCOMPARE(testWaveModel.getValue(10, 10), expected);
            testWaveModel.propagate();
            delta = 0.171875;
            expected += delta; // 0.3125
            QCOMPARE(testWaveModel.getValue(10, 10), expected);
            testWaveModel.propagate();
            delta = -0.26171875;
            expected += delta; // 0.05078125
            QCOMPARE(testWaveModel.getValue(10, 10), expected);
            testWaveModel.propagate();
            delta = -0.16796875;
            expected += delta; // -0.1171875
            QCOMPARE(testWaveModel.getValue(10, 10), expected);
            testWaveModel.propagate();
            delta = 0.04351806640625;
            expected += delta; // -0.07366943359375
            QCOMPARE(testWaveModel.getValue(10, 10), expected);
        }
        void WaveModelTest::MethodPropagateWorksAcrossSpace() {
            WaveModel testWaveModel(50, 50);
            double expected = 1.0;
            unsigned int x = 10;
            unsigned int y = 10;
            testWaveModel.setSourceValue(x, y, expected);
            QCOMPARE(testWaveModel.getValue(x, y), expected);
            x++;
            for (unsigned int i = 0; i < 10; i++, x++) {
                QCOMPARE(testWaveModel.getValue(x, y), 0.0);
                testWaveModel.propagate();
                expected /= 4.0;
                QCOMPARE(testWaveModel.getValue(x, y), expected);
            }
        }
    }
}

using ews::test::WaveModelTest;
QTEST_APPLESS_MAIN(WaveModelTest)
