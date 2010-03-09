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

#include "PrecomputedPotentialTest.h"
#include "ConstantPotential.h"
#include "WallPotential.h"
#include "CompositePotential.h"
#include <osg/Vec2>

namespace ews {
    namespace test {
        using osg::Vec2;
        
        void PrecomputedPotentialTest::initTestCase() { /* do nothing */ }
        void PrecomputedPotentialTest::cleanupTestCase() { /* do nothing */ }
        void PrecomputedPotentialTest::init() { /* do nothing */ }
        void PrecomputedPotentialTest::cleanup() { /* do nothing */ }
        
        void PrecomputedPotentialTest::DefaultConstructorIsZero() {
            PrecomputedPotential p;
            // Tests that the potential is zero (unsigned int values shouldn't matter)
            QCOMPARE(p.getPotential(2, 2), 0.0);
        }
        
        void PrecomputedPotentialTest::CanHandleSingleConstantPotential() {
            const double value = 3.14159;
            counted_ptr<const Potential> constP = counted_ptr<const Potential>(new ConstantPotential(value));
            PrecomputedPotential p(constP, 10, 10);
            QCOMPARE(p.getPotential(2, 2), value);
            QCOMPARE(p.getPotential(11, 2), 0.0);
            QCOMPARE(p.getPotential(2, 11), 0.0);
        }
        
        void PrecomputedPotentialTest::CanHandlePotentialOutsideItsBounds() {
            const double size = 50;
            counted_ptr<const Potential> east = counted_ptr<const Potential>(new WallPotential(Vec2(size, 0.0), Vec2(size, size)));
            counted_ptr<const Potential> north = counted_ptr<const Potential>(new WallPotential(Vec2(0.0, size), Vec2(size, size)));
            counted_ptr<const Potential> south = counted_ptr<const Potential>(new WallPotential(Vec2(0.0, 0.0), Vec2(size, 0.0)));
            counted_ptr<const Potential> west = counted_ptr<const Potential>(new WallPotential(Vec2(0.0, 0.0), Vec2(0.0, size)));
            CompositePotential* northSouth = new CompositePotential();
            northSouth->addPotential(north);
            northSouth->addPotential(south);
            CompositePotential* eastWest = new CompositePotential();
            eastWest->addPotential(east);
            eastWest->addPotential(west);
            CompositePotential* world = new CompositePotential();
            counted_ptr<const Potential> p = counted_ptr<const Potential>(northSouth);
            world->addPotential(p);
            p = counted_ptr<const Potential>(eastWest);
            world->addPotential(p);
            p = counted_ptr<const Potential>(world);
            // Notice that bounds are slightly smaller than potential being passed to it
            PrecomputedPotential pre(p, static_cast<unsigned int>(size - 1), static_cast<unsigned int>(size - 1));
            QCOMPARE(pre.getPotential(0.0, size / 2), 100.0);
            QCOMPARE(pre.getPotential(size / 2, 0.0), 100.0);
            QCOMPARE(pre.getPotential(size / 2, size), 0.0); // Because this is outside the precomputed bounds
            QCOMPARE(pre.getPotential(size, size / 2), 0.0); // Because this is outside the precomputed bounds
            QCOMPARE(pre.getPotential(size / 2, size - 2), 100.0); // However, the wall can be felt from here
            QCOMPARE(pre.getPotential(size - 2, size / 2), 100.0);
            QCOMPARE(pre.getPotential(size / 2, size / 2), 0.0);
        }
    }
}

using ews::test::PrecomputedPotentialTest;
QTEST_APPLESS_MAIN(PrecomputedPotentialTest)
