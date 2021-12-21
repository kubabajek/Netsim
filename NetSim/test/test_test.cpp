//#include "gmock/gmock.h"
#include "gtest/gtest.h"


TEST(Test, JakisTest1) {

    int a=1;
    int b=2;

    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 2);
}

TEST(Test, JakisTest2) {

    int a=1;
    int b=2;

    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 3);
}