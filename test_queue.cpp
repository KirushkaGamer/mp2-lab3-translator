#include <gtest/gtest.h>
#include "translator.h"

TEST(QueueTest, NewQueueIsEmpty) {
    Queue<int> q;
    EXPECT_TRUE(q.empty());
    EXPECT_EQ(q.size(), 0u);
}

TEST(QueueTest, FIFOOrder) {
    Queue<int> q;
    q.push(10);
    q.push(20);
    q.push(30);

    EXPECT_EQ(q.front(), 10);
    EXPECT_EQ(q.back(), 30);
    EXPECT_EQ(q.pop(), 10);
    EXPECT_EQ(q.pop(), 20);
    EXPECT_EQ(q.pop(), 30);
    EXPECT_TRUE(q.empty());
}

TEST(QueueTest, CircularWrapAroundWithoutResize) {
    Queue<int> q(4); // Буфер емкостью 4
    q.push(1);
    q.push(2);
    q.push(3);
    EXPECT_EQ(q.pop(), 1);
    EXPECT_EQ(q.pop(), 2);

    // Теперь head сдвинут, добавляем еще элементы с переходом через границу массива
    q.push(4);
    q.push(5);
    q.push(6);

    EXPECT_EQ(q.size(), 4u);
    EXPECT_EQ(q.capacity(), 4u); // Расширения быть не должно
    EXPECT_EQ(q.pop(), 3);
    EXPECT_EQ(q.pop(), 4);
    EXPECT_EQ(q.pop(), 5);
    EXPECT_EQ(q.pop(), 6);
    EXPECT_TRUE(q.empty());
}

TEST(QueueTest, AutomaticExpansionWhenFullAfterWrapAround) {
    Queue<int> q(3);
    q.push(1);
    q.push(2);
    q.pop(); // Извлекли 1, head = 1
    q.push(3);
    q.push(4); // Теперь буфер полон в кольцевом режиме: [4, 2, 3]
    q.push(5); // Должно произойти корректное расширение буфера

    EXPECT_EQ(q.size(), 4u);
    EXPECT_GE(q.capacity(), 6u);
    EXPECT_EQ(q.pop(), 2);
    EXPECT_EQ(q.pop(), 3);
    EXPECT_EQ(q.pop(), 4);
    EXPECT_EQ(q.pop(), 5);
}

TEST(QueueTest, PopAndFrontOnEmptyQueueThrow) {
    Queue<int> q;
    EXPECT_THROW(q.pop(), std::out_of_range);
    EXPECT_THROW(q.front(), std::out_of_range);
    EXPECT_THROW(q.back(), std::out_of_range);
}