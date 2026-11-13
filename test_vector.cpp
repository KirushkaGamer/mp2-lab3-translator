#include <gtest/gtest.h>
#include "translator.h"

TEST(VectorTest, DefaultConstructorIsEmpty) {
    Vector<int> v;
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0u);
}

TEST(VectorTest, PushBackAndIndexing) {
    Vector<int> v;
    for (int i = 0; i < 20; ++i) {
        v.push_back(i * 10);
    }
    EXPECT_EQ(v.size(), 20u);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[10], 100);
    EXPECT_EQ(v.back(), 190);
}

TEST(VectorTest, PopBackReducesSize) {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.pop_back();
    EXPECT_EQ(v.size(), 1u);
    EXPECT_EQ(v.back(), 1);
}

TEST(VectorTest, PopBackOnEmptyThrows) {
    Vector<int> v;
    EXPECT_THROW(v.pop_back(), std::out_of_range);
}

TEST(VectorTest, AtOutOfRangeThrows) {
    Vector<int> v(5, 42);
    EXPECT_NO_THROW(v.at(4));
    EXPECT_THROW(v.at(5), std::out_of_range);
}

TEST(VectorTest, CopyConstructorAndAssignmentMakeDeepCopy) {
    Vector<int> v1;
    v1.push_back(10);
    v1.push_back(20);

    Vector<int> v2(v1);
    v2[0] = 99;
    EXPECT_EQ(v1[0], 10);
    EXPECT_EQ(v2[0], 99);

    Vector<int> v3;
    v3 = v1;
    v3[1] = 88;
    EXPECT_EQ(v1[1], 20);
    EXPECT_EQ(v3[1], 88);
}