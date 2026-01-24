#include <gtest/gtest.h>

#include "Largest.h"

class LargestTest: public ::testing::Test {
protected:
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.

    // virtual void SetUp() will be called before each test is run.  You
    // should define it if you need to initialize the variables.
    // Otherwise, this can be skipped.
    virtual void SetUp() {
    }

    // virtual void TearDown() will be called after each test is run.
    // You should define it if there is cleanup work to do.  Otherwise,
    // you don't have to provide it.
    virtual void TearDown() {
    }
};

TEST_F(LargestTest, SimpleTest) {
    std::vector<int> some_numbers = std::vector<int>();
    some_numbers.push_back(3);
    some_numbers.push_back(5);
    some_numbers.push_back(-11);

    int max = largest2(&some_numbers);

    EXPECT_EQ(5, max);
}


#include "Fighterplane.h"



class FighterplaneTest: public ::testing::Test {
protected:
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.

    // virtual void SetUp() will be called before each test is run.  You
    // should define it if you need to initialize the variables.
    // Otherwise, this can be skipped.
    virtual void SetUp() {
    }

    // virtual void TearDown() will be called after each test is run.
    // You should define it if there is cleanup work to do.  Otherwise,
    // you don't have to provide it.
    virtual void TearDown() {
    }

};

TEST_F(FighterplaneTest, CallSignTest) {
    Fighterplane test = Fighterplane("some_callsign 123", 1, 2, 3);

    EXPECT_EQ("some_callsign 123", test.getCallSign());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
