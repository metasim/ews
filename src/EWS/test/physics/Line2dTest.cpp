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

#include "Line2dTest.h"

namespace ews {
    namespace test {
        void Line2dTest::initTestCase() { /* do nothing */ }
        void Line2dTest::cleanupTestCase() { /* do nothing */ }
        void Line2dTest::init() { /* do nothing */ }
        void Line2dTest::cleanup() { /* do nothing */ }
        
        void Line2dTest::ConstructorWorks() {
            QCOMPARE(_l.getStart().x(), _x1);
            QCOMPARE(_l.getStart().y(), _y1);
            QCOMPARE(_l.getEnd().x(), _x2);
            QCOMPARE(_l.getEnd().y(), _y2);
        }
        
        void Line2dTest::MethodsGetSetStartWork() {
            double x = 13542.523;
            double y = -99922.3452;
            Point2d newStart(x, y);
            _l.setStart(newStart);
            QCOMPARE(_l.getStart().x(), x);
            QCOMPARE(_l.getStart().y(), y);
        }
        
        void Line2dTest::MethodsGetSetEndWork() {
            double x = 13542.523;
            double y = -99922.3452;
            Point2d newEnd(x, y);
            _l.setEnd(newEnd);
            QCOMPARE(_l.getEnd().x(), x);
            QCOMPARE(_l.getEnd().y(), y);
        }
        
        void Line2dTest::MethodEpsilonWorks() {
            const Point2d p1(0.0, 3.0);
            const Point2d p2(4.0, 0.0);
            const Line2d l(p1, p2);
            QCOMPARE(l.epislon(), 5e-6);
        }
        
        void Line2dTest::MethodPtSegProjectionWorks() {
            const Point2d p1(0.0, 0.0);
            const Point2d p2(4.0, 0.0);
            const Line2d l(p1, p2);
            const Point2d ptToProject1(2.0, 3.1253);
            Point2d projectedPt;
            QCOMPARE(l.ptSegProjection(ptToProject1, projectedPt), true);
            QCOMPARE(projectedPt.x(), 2.0);
            QCOMPARE(projectedPt.y(), 0.0);
            const Point2d ptToProject2(4.0, 3.1253);
            QCOMPARE(l.ptSegProjection(ptToProject2, projectedPt), true);
            QCOMPARE(projectedPt.x(), 4.0);
            QCOMPARE(projectedPt.y(), 0.0);
            const Point2d ptToProject3(4.01, 3.1253);
            QCOMPARE(l.ptSegProjection(ptToProject3, projectedPt), false);
            QCOMPARE(projectedPt.x(), 4.01);
            QCOMPARE(projectedPt.y(), 0.0);
            // TODO: Create a more rigorous test
        }
    }
}

using ews::test::Line2dTest;
QTEST_APPLESS_MAIN(Line2dTest)
