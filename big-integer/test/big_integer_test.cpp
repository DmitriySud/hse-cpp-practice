#include <gtest/gtest.h>

#include <big_integer.hpp>

TEST(BigInt, Constructor) {
    BigInteger zero;
    BigInteger another_zero(0);

    ASSERT_TRUE(zero == another_zero);
}

TEST(BigInt, IntToString) {
    BigInteger value{12345};
    EXPECT_EQ("12345", value.ToString());
}

TEST(BigInt, CopyConstructor) {
    BigInteger value{12345};
    EXPECT_EQ("12345", BigInteger(value).ToString());
}

TEST(BigInt, MoveConstructor) {
    BigInteger value{12345};
    BigInteger other_value(std::move(value));
    EXPECT_EQ("12345", other_value.ToString());
}

TEST(BigInt, PlusOperator) {
    BigInteger one(12345);
    BigInteger two(12345);

    EXPECT_EQ("24690", (one + two).ToString());

    one = 12345;
    two = -12345;

    EXPECT_EQ("0", (one + two).ToString());
    EXPECT_EQ("0", (two + one).ToString());

    one = 12345;
    two = -12340;
    EXPECT_EQ("5", (two + one).ToString());

    two = two + BigInteger(-10);
    EXPECT_EQ("-5", (two + one).ToString());
}

TEST(BigInt, MinusOperation) {
    BigInteger one(12345);
    BigInteger two(12345);

    EXPECT_EQ("0", (one - two).ToString());
    EXPECT_EQ("0", (two - one).ToString());

    one = 12345;
    two = -12345;

    EXPECT_EQ("24690", (one - two).ToString());
    EXPECT_EQ("-24690", (two - one).ToString());

    one = -12345;
    two = 12340;
    EXPECT_EQ("-24685", (one - two).ToString());
}

TEST(BigInt, LessOperation) {
    BigInteger big_positive(12345);
    BigInteger small_positive(123);

    BigInteger big_negative(-12345);
    BigInteger small_negative(-123);

    ASSERT_TRUE(small_positive < big_positive);
    ASSERT_FALSE(big_positive < small_positive);
    ASSERT_TRUE(big_negative < small_negative);
    ASSERT_TRUE(big_negative < small_positive);
    ASSERT_FALSE(small_positive < big_negative);
    ASSERT_FALSE(small_positive < small_positive);
}

TEST(BigInt, GreaterOperation) {
    BigInteger big_positive(12345);
    BigInteger small_positive(123);

    BigInteger big_negative(-12345);
    BigInteger small_negative(-123);

    ASSERT_FALSE(small_positive > big_positive);
    ASSERT_TRUE(big_positive > small_positive);
    ASSERT_FALSE(big_negative > small_negative);
    ASSERT_FALSE(big_negative > small_positive);
    ASSERT_TRUE(small_positive > big_negative);
    ASSERT_FALSE(small_positive > small_positive);
}

TEST(BigInt, LessEqOperation) {
    BigInteger big_positive(12345);
    BigInteger small_positive(123);

    BigInteger big_negative(-12345);
    BigInteger small_negative(-123);

    ASSERT_TRUE(small_positive <= big_positive);
    ASSERT_FALSE(big_positive <= small_positive);
    ASSERT_TRUE(big_negative <= small_negative);
    ASSERT_TRUE(big_negative <= small_positive);
    ASSERT_FALSE(small_positive <= big_negative);
    ASSERT_TRUE(small_positive <= small_positive);
}

TEST(BigInt, GreaterEqOperation) {
    BigInteger big_positive(12345);
    BigInteger small_positive(123);

    BigInteger big_negative(-12345);
    BigInteger small_negative(-123);

    ASSERT_FALSE(small_positive >= big_positive);
    ASSERT_TRUE(big_positive >= small_positive);
    ASSERT_FALSE(big_negative >= small_negative);
    ASSERT_FALSE(big_negative >= small_positive);
    ASSERT_TRUE(small_positive >= big_negative);
    ASSERT_TRUE(small_positive >= small_positive);
}

TEST(BigInt, EqOperation) {
    BigInteger big_positive(12345);
    BigInteger small_positive(123);

    BigInteger big_negative(-12345);
    BigInteger small_negative(-123);

    ASSERT_TRUE(big_positive == big_positive);
    ASSERT_FALSE(big_positive == small_positive);

    ASSERT_TRUE(big_negative == big_negative);
    ASSERT_FALSE(small_negative == small_positive);

    ASSERT_FALSE(big_negative == big_positive);
}
