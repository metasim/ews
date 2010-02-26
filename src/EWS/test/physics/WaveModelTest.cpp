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
        
        void WaveModelTest::MethodPropagateWorks() {
            WaveModel testWaveModel(50, 50);
            // Tests that the average value for the range x = [0..4], y = [0..4] is zero
            QCOMPARE(testWaveModel.computeAverageValue(2, 2, 2), 0.0);
        }
    }
}

using ews::test::WaveModelTest;
QTEST_APPLESS_MAIN(WaveModelTest)
