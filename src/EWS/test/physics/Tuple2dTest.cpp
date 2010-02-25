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

#include "Tuple2dTest.h"

namespace ews {
    namespace test {
        void Tuple2dTest::initTestCase() { /* do nothing */ }
        void Tuple2dTest::cleanupTestCase() { /* do nothing */ }
        void Tuple2dTest::init() { /* do nothing */ }
        void Tuple2dTest::cleanup() { /* do nothing */ }
        
        void Tuple2dTest::DefaultConstructorIsZero() {
            const Tuple2d t;
            QCOMPARE(static_cast<double>(0.0), t.x());
            QCOMPARE(static_cast<double>(0.0), t.y());
        }
        
        void Tuple2dTest::XYConstructorYieldsXY() {
            const double x = 0.71234;
            const double y = 13.89023;
            const Tuple2d t(x, y);
            QCOMPARE(x, t.x());
            QCOMPARE(y, t.y());
        }

        void Tuple2dTest::InterpolatedConstructorIsInterpolated() {
            const Tuple2d t1(0.0, 4.8);
            const Tuple2d t2(5.0, 2.4);
            const Tuple2d interpolated(t1, t2, 0.5);
            QCOMPARE(static_cast<double>(2.5), interpolated.x());
            QCOMPARE(static_cast<double>(3.6), interpolated.y());
        }

        void Tuple2dTest::CopyConstructorCopies() {
            const double x = 0.71234;
            const double y = 13.89023;
            const Tuple2d t(x, y);
            const Tuple2d copied(t);
            QCOMPARE(x, copied.x());
            QCOMPARE(y, copied.y());
        }

        void Tuple2dTest::MethodComputeEpsilonWorks() {
            const Tuple2d t1(0.0, 3.0);
            const Tuple2d t2(4.0, 0.0);
            QCOMPARE(5e-6, Tuple2d::computeEpsilon(t1, t2));
        }
        
        void Tuple2dTest::OperatorAssignmentWorks() {
            const double x = 0.71234;
            const double y = 13.89023;
            const Tuple2d t(x, y);
            Tuple2d copied(2.0, 3.0);
            copied = t;
            QCOMPARE(x, copied.x());
            QCOMPARE(y, copied.y());
        }
        
        void Tuple2dTest::OperatorSubtractionWorks() {
            const Tuple2d t1(0.0, 4.8);
            const Tuple2d t2(5.0, 2.4);
            const Tuple2d t(t2 - t1);
            QCOMPARE(static_cast<double>(5.0), t.x());
            QCOMPARE(static_cast<double>(-2.4), t.y());
        }
    }
}

using ews::test::Tuple2dTest;
QTEST_APPLESS_MAIN(Tuple2dTest)
