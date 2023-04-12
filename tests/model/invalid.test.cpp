#include <gtest/gtest.h>
#include "parser.hpp"

TEST(invalid, empty_expr)
{
    Expression_Parser parser;
    EXPECT_FALSE(parser.parse("  ").expression);
}

TEST(invalid, not_supported_operator)
{
    Expression_Parser parser;
    EXPECT_FALSE(parser.parse("3 p 5").expression);
}

TEST(invalid, wrong_closing_parenthesis_number)
{
    Expression_Parser parser;
    EXPECT_FALSE(parser.parse("(3+5))").expression);
}

TEST(invalid, wrong_opening_parenthesis_number)
{
    Expression_Parser parser;
    EXPECT_FALSE(parser.parse("((3+5)").expression);
}

TEST(invalid, two_member_without_operator)
{
    Expression_Parser parser;
    EXPECT_FALSE(parser.parse("3 5").expression);
}

TEST(invalid, two_member_with_parenthesis_without_operator)
{
    Expression_Parser parser;
    EXPECT_FALSE(parser.parse("(3+5)(4)").expression);
}