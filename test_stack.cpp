#include <gtest/gtest.h>
#include "translator.h"

TEST(StackTest, NewStackIsEmpty) {
    Stack<int> s;
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(), 0u);
}

TEST(StackTest, PushAndTop) {
    Stack<int> s;
    s.push(5);
    EXPECT_FALSE(s.empty());
    EXPECT_EQ(s.top(), 5);
    s.push(10);
    EXPECT_EQ(s.top(), 10);
    EXPECT_EQ(s.size(), 2u);
}

TEST(StackTest, LIFOOrderOnPop) {
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);

    EXPECT_EQ(s.pop(), 3);
    EXPECT_EQ(s.pop(), 2);
    EXPECT_EQ(s.pop(), 1);
    EXPECT_TRUE(s.empty());
}

TEST(StackTest, PopAndTopOnEmptyStackThrow) {
    Stack<double> s;
    EXPECT_THROW(s.pop(), std::out_of_range);
    EXPECT_THROW(s.top(), std::out_of_range);
}