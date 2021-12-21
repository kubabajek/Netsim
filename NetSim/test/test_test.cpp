//#include "gmock/gmock.h"
#include "gtest/gtest.h"


TEST(ZbiorTestow1, Przyklad1) {

    int a=1;
    int b=2;

    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 2);
}

TEST(ZbiorTestow1, Przyklad2) {

    int a=1;
    int b=2;

    EXPECT_EQ(a, 1);
    EXPECT_EQ(a+b, 3);
}

TEST(ZbiorTestow1, Przyklad3) {

    int a=1;
    int b=2;

    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 2);
}

TEST(ZbiorTestow2, Przyklad1) {

    int a=5;
    int b=3;

    EXPECT_EQ(a, 5);
    EXPECT_EQ(b, 3);
}

TEST(ZbiorTestow2, Przyklad2) {

    int a=5;
    int b=3;
    std::cout << "Welcome in spiulkolot!" << std::endl;


EXPECT_EQ(a-b, 2);
    EXPECT_EQ(b, 3);
}