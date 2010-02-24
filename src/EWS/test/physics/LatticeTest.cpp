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

#include "Lattice.h"

#include <gtest/gtest.h>

namespace {
    
    // The fixture for testing class Lattice.
    class LatticeTest : public ::testing::Test {
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.
        
        LatticeTest() {
            // You can do set-up work for each test here.
        }
        
        virtual ~LatticeTest() {
            // You can do clean-up work that doesn't throw exceptions here.
        }
        
        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:
        
        virtual void SetUp() {
            // Code here will be called immediately after the constructor (right
            // before each test).
        }
        
        virtual void TearDown() {
            // Code here will be called immediately after each test (right
            // before the destructor).
        }
        
        // Objects declared here can be used by all tests in the test case for LatticeTest.
    };
    
    // Tests that the Lattice::computeAverageValue() method works for normal cases.
    TEST_F(LatticeTest, MethodComputeAverageValueWorksForNormalCases) {
        Lattice testLattice(5, 5);
        // Tests that the average value for the range x = [0..4], y = [0..4] is zero
        EXPECT_EQ(0.0, testLattice.computeAverageValue(2, 2, 2));
        testLattice.setValue(2, 2, 9.0);
        // "Averaging" range x = 2, y = 2
        EXPECT_EQ(9.0, testLattice.computeAverageValue(2, 2, 0));
        // Averaging range x = [1..3], y = [1..3]
        EXPECT_EQ(1.0, testLattice.computeAverageValue(2, 2, 1));
        // Averaging range x = [0..4], y = [0..4]
        EXPECT_EQ(9.0/25.0, testLattice.computeAverageValue(2, 2, 2));
    }

    // Tests that the Lattice::computeAverageValue() method works for cases where windows go outside bounds.
    TEST_F(LatticeTest, MethodComputeAverageValueWorksForWindowsGoingOutsideBounds) {
        Lattice testLattice(5, 5);
        // Tests that the average value for the range x = [0..2], y = [0..2] is zero
        EXPECT_EQ(0.0, testLattice.computeAverageValue(0, 0, 2));
        testLattice.setValue(2, 2, 9.0);
        // Averaging range x = [0..1], y = [0..1]
        EXPECT_EQ(0.0, testLattice.computeAverageValue(0, 0, 1));
        // Averaging range x = [0..2], y = [0..2]
        EXPECT_EQ(1.0, testLattice.computeAverageValue(0, 0, 2));
        // Averaging range x = [0..2], y = [0..4]
        EXPECT_EQ(9.0/15.0, testLattice.computeAverageValue(0, 2, 2));
        // Averaging range x = [0..2], y = [2..4]
        EXPECT_EQ(1.0, testLattice.computeAverageValue(0, 4, 2));
    }
    
    // Tests that the Lattice::getWidth() method works
    TEST_F(LatticeTest, MethodGetWidthWorks) {
        EXPECT_EQ(5, Lattice(5, 3).getWidth());
        EXPECT_EQ(0, Lattice(0, 1).getWidth());
        EXPECT_EQ(1, Lattice(1, 0).getWidth());
    }
    
    // Tests that the Lattice::getLength() method works
    TEST_F(LatticeTest, MethodGetLengthWorks) {
        EXPECT_EQ(3, Lattice(5, 3).getLength());
        EXPECT_EQ(0, Lattice(0, 1).getLength()); // s/b zero because of implementation detail
        EXPECT_EQ(0, Lattice(1, 0).getLength());
    }

    // Tests that the Lattice::setSize() method works
    TEST_F(LatticeTest, MethodSetSizeWorks) {
        Lattice testLattice(5, 5);
        testLattice.setValue(2, 2, 9.0);
        testLattice.setSize(10, 20);
        EXPECT_EQ(10, testLattice.getWidth());
        EXPECT_EQ(20, testLattice.getLength());
        EXPECT_EQ(0, testLattice.getValue(2, 2));
        testLattice.setSize(0, 0);
    }

    // Tests that Lattice::setValue() and Lattice::getValue() method works
    TEST_F(LatticeTest, MethodSetGetValueWorks) {
        Lattice testLattice(5, 5);
        testLattice.setValue(2, 2, 9.0);
        EXPECT_EQ(9.0, testLattice.getValue(2, 2));
    }


//    // Tests that Foo does Xyz.
//    TEST_F(LatticeTest, DoesXyz) {
//        // Exercises the Xyz feature of Foo.
//    }
    
}  // namespace

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
