#include <gtest/gtest.h>
#include "translator.h"

TEST(TranslatorTest, BasicArithmetic) {
    EXPECT_DOUBLE_EQ(Translator::calculate("2 + 3"), 5.0);
    EXPECT_DOUBLE_EQ(Translator::calculate("10 - 4"), 6.0);
    EXPECT_DOUBLE_EQ(Translator::calculate("6 * 7"), 42.0);
    EXPECT_DOUBLE_EQ(Translator::calculate("15 / 2"), 7.5);
}

TEST(TranslatorTest, OperatorPrecedence) {
    EXPECT_DOUBLE_EQ(Translator::calculate("2 + 3 * 4"), 14.0);
    EXPECT_DOUBLE_EQ(Translator::calculate("20 - 10 / 2 + 3 * 2"), 21.0);
}

TEST(TranslatorTest, ParenthesesOverridePrecedence) {
    EXPECT_DOUBLE_EQ(Translator::calculate("(2 + 3) * 4"), 20.0);
    EXPECT_DOUBLE_EQ(Translator::calculate("((10 - 2) / (1 + 3)) * 5"), 10.0);
}

TEST(TranslatorTest, FloatingPointNumbers) {
    EXPECT_NEAR(Translator::calculate("3.5 * 2.0 + 1.25"), 8.25, 1e-9);
    EXPECT_NEAR(Translator::calculate("0.1 + 0.2"), 0.3, 1e-9);
}

TEST(TranslatorTest, UnaryMinusAndPlus) {
    EXPECT_DOUBLE_EQ(Translator::calculate("-5 + 3"), -2.0);
    EXPECT_DOUBLE_EQ(Translator::calculate("10 * (-2)"), -20.0);
    EXPECT_DOUBLE_EQ(Translator::calculate("-(-4)"), 4.0);
    EXPECT_DOUBLE_EQ(Translator::calculate("+5 + (-2)"), 3.0);
}

TEST(TranslatorTest, PostfixConversionOrder) {
    EXPECT_EQ(Translator::toPostfixString("2 + 3 * 4"), "2 3 4 * +");
    EXPECT_EQ(Translator::toPostfixString("(2 + 3) * 4"), "2 3 + 4 *");
}

TEST(TranslatorTest, DivisionByZeroThrows) {
    EXPECT_THROW(Translator::calculate("10 / 0"), std::domain_error);
    EXPECT_THROW(Translator::calculate("5 / (3 - 3)"), std::domain_error);
}

TEST(TranslatorTest, InvalidSyntaxAndBracketsThrow) {
    EXPECT_THROW(Translator::calculate(""), std::invalid_argument);
    EXPECT_THROW(Translator::calculate("   "), std::invalid_argument);
    EXPECT_THROW(Translator::calculate("2 + * 3"), std::invalid_argument);
    EXPECT_THROW(Translator::calculate("(2 + 3"), std::invalid_argument);
    EXPECT_THROW(Translator::calculate("2 + 3)"), std::invalid_argument);
    EXPECT_THROW(Translator::calculate("()"), std::invalid_argument);
    EXPECT_THROW(Translator::calculate("2..5 + 1"), std::invalid_argument);
    EXPECT_THROW(Translator::calculate("2 + a"), std::invalid_argument);
}