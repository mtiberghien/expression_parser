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

TEST(combined, parse_3_plus_5_mult_3_minus_3)
{
    Expression_Parser parser;
    EXPECT_EQ(3+5*3-3, parser.parse("3+5*3-3").expression->evaluate());
}

TEST(combined, parse_3_minus_5_mult_3_plus_2)
{
    Expression_Parser parser;
    EXPECT_EQ(3-5*3+2, parser.parse("3-5*3+2").expression->evaluate());
}

TEST(combined, parse_1_plus_2_mult_3_exp_2)
{
    Expression_Parser parser;
    EXPECT_EQ(1+2*pow(3,2), parser.parse("1+2*3^2").expression->evaluate());
}

TEST(combined, parse_1_plus_2_mult_3_exp_2_minus_3_mult_4)
{
    Expression_Parser parser;
    EXPECT_EQ(1+2*pow(3,2)-3*4, parser.parse("1+2*3^2-3*4").expression->evaluate());
}

