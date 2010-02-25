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

#include "WallPotentialTest.h"

namespace ews {
    namespace test {
        void WallPotentialTest::initTestCase() { /* do nothing */ }
        void WallPotentialTest::cleanupTestCase() { /* do nothing */ }
        void WallPotentialTest::init() { /* do nothing */ }
        void WallPotentialTest::cleanup() { /* do nothing */ }
        
        void WallPotentialTest::DefaultConstructorIsPointWall() {
            WallPotential p;
            // Tests that the potential is zero away from origin
            QCOMPARE(p.getPotential(2, 2, 2), 0.0);
            // Tests that the potential is 100 at origin
            QCOMPARE(p.getPotential(0, 0, 2), 100.0);
        }
        
        void WallPotentialTest::PreferredConstructorWorks() {
            WallPotential p(Point2d(0.0, 0.0), Point2d(5.0, 5.0));
            QCOMPARE(p.getPotential(2, 2, 2), 100.0);
            QCOMPARE(p.getPotential(6, 6, 2), 0.0);
            QCOMPARE(p.getPotential(2, 3, 2), 100.0);
            QCOMPARE(p.getPotential(3, 2, 2), 100.0);
            QCOMPARE(p.getPotential(4, 2, 2), 100.0);
            QCOMPARE(p.getPotential(5, 0, 2), 0.0);
        }
        
        void WallPotentialTest::MethodsGetSetSrcPointWork() {
            WallPotential p(Point2d(0.0, 0.0), Point2d(5.0, 5.0));
            p.setSrcPoint(Point2d(10.0, -10.0));
            QCOMPARE(p.getSrcPoint().x(), 10.0);
            QCOMPARE(p.getSrcPoint().y(), -10.0);
        }
        
        void WallPotentialTest::MethodsGetSetDstPointWork() {
            WallPotential p(Point2d(0.0, 0.0), Point2d(5.0, 5.0));
            p.setDstPoint(Point2d(10.0, -10.0));
            QCOMPARE(p.getDstPoint().x(), 10.0);
            QCOMPARE(p.getDstPoint().y(), -10.0);
        }
        
        void WallPotentialTest::MethodGetSetThicknessWorks() {
            WallPotential p(Point2d(0.0, 0.0), Point2d(5.0, 5.0));
            p.setThickness(1);
            QCOMPARE(p.getThickness(), static_cast<unsigned int>(1));
            QCOMPARE(p.getPotential(2, 2, 2), 100.0);
            QCOMPARE(p.getPotential(3, 2, 2), 100.0);
            QCOMPARE(p.getPotential(4, 2, 2), 0.0);
        }
    }
}

using ews::test::WallPotentialTest;
QTEST_MAIN(WallPotentialTest)
