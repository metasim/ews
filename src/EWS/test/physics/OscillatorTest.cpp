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

#include "OscillatorTest.h"
#include <cmath>
using std::abs;

namespace ews {
    namespace test {
        void OscillatorTest::initTestCase() { /* do nothing */ }
        void OscillatorTest::cleanupTestCase() { /* do nothing */ }
        void OscillatorTest::init() { /* do nothing */ }
        void OscillatorTest::cleanup() { /* do nothing */ }
        
        void OscillatorTest::DefaultConstructor() {
            unsigned int width = 50;
            unsigned int length = 100;
            WaveModel waveModel(width, length);
            Oscillator testOscillator(waveModel);
//            QCOMPARE(testOscillator.getValue(), 0.0);
            QVERIFY(abs(testOscillator.getValue()) < 1e-16);
            // I don't like that we're not precise enough for qtest's QCOMPARE
            if (abs(testOscillator.getValue()) > 1e-18) {
                QWARN("Value was greater than expected");
            }
            QCOMPARE(testOscillator.getRadius(), DEFAULT_RADIUS);
            QCOMPARE(testOscillator.getPeriod(), DEFAULT_PERIOD);            
            QCOMPARE(testOscillator.x(), DEFAULT_X);            
            QCOMPARE(testOscillator.y(), length / 2);            
            QCOMPARE(testOscillator.getAmplitude(), DEFAULT_AMPLITUDE);            
            QCOMPARE(testOscillator.getTime(), 0.0);            
            QCOMPARE(testOscillator.getOscillateStatus(), false);            
        }
    }
}

using ews::test::OscillatorTest;
QTEST_APPLESS_MAIN(OscillatorTest)
