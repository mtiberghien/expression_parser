#include <gtest/gtest.h>
#include "parser.hpp"

TEST(combined, parse_3_minus_4_plus_5)
{
    Expression_Parser parser;
    EXPECT_EQ(3-4+5, parser.parse("3 -4 +5  ").expression->evaluate());
}

TEST(combined, parse_minus_3_plus_4_minus_5)
{
    Expression_Parser parser;
    EXPECT_EQ(-3+4-5, parser.parse("-3+4-5").expression->evaluate());
}

TEST(combined, parse_3_plus_5_mult_3)
{
    Expression_Parser parser;
    EXPECT_EQ(3+5*3, parser.parse("3+5*3").expression->evaluate());
}