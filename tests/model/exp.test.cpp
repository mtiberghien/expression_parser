#include <gtest/gtest.h>
#include "parser.hpp"

TEST(exp, parse_const_3_exp_4)
{
    Expression_Parser parser;
    EXPECT_EQ(pow(3,4), parser.parse("3^4").expression->evaluate());
}

TEST(exp, parse_const_2_exp_33)
{
    Expression_Parser parser;
    EXPECT_EQ(pow(2,33), parser.parse("2^33").expression->evaluate());
}