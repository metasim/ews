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

#include "Vector2dTest.h"
#include "Point2d.h"

namespace ews {
    namespace test {
        void Vector2dTest::initTestCase() { /* do nothing */ }
        void Vector2dTest::cleanupTestCase() { /* do nothing */ }
        void Vector2dTest::init() { /* do nothing */ }
        void Vector2dTest::cleanup() { /* do nothing */ }
        
        void Vector2dTest::CopyConstructorTakesPoints() {
            const double x = -1234.51235;
            const double y = 3235235253.2343;
            const Vector2d v(Point2d(x, y));
            QCOMPARE(v.x(), x);
            QCOMPARE(v.y(), y);
        }
        
        void Vector2dTest::MethodLengthWorks() {
            const Vector2d v(5.0, 12.0);
            QCOMPARE(v.length(), 13.0);
        }
        
        void Vector2dTest::MethodLengthSqWorks() {
            const Vector2d v(1.0, 2.1);
            QCOMPARE(v.lengthSq(), 5.41);
        }
        
        void Vector2dTest::MethodDotWorks() {
            const Vector2d v1(0.0, 3.0);
            const Vector2d v2(4.0, 0.0);
            QCOMPARE(v1.dot(v2), 0.0);
            const Vector2d v3(0.0, 4.0);
            QCOMPARE(v1.dot(v3), 12.0);
            const Vector2d v4(0.0, -4.0);
            QCOMPARE(v1.dot(v4), -12.0);
            const Vector2d v5(3.14159, -4.0);
            QCOMPARE(v1.dot(v5), -12.0);
        }
    }
}

using ews::test::Vector2dTest;
QTEST_APPLESS_MAIN(Vector2dTest)
