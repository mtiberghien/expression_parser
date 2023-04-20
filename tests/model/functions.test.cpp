#include <gtest/gtest.h>
#include "parser.hpp"

TEST(functions, parse_max_3_4)
{
    Expression_Parser parser;
    auto result = parser.parse("max(3,4)");
    ASSERT_TRUE(result);
    EXPECT_EQ(max(3,4), result.expression->evaluate());
}

TEST(functions, parse_max_var_1_4)
{
    Expression_Parser parser;
    MemoryDataContext dc;
    int var_1 = 3;
    dc.add_or_set("var_1", var_1);
    auto result = parser.parse("max(${var_1},4)");
    ASSERT_TRUE(result);
    EXPECT_EQ(max(var_1,4), result.expression->evaluate(&dc));
    var_1=6;
    dc.add_or_set("var_1", var_1);
    EXPECT_EQ(max(var_1,4), result.expression->evaluate(&dc));
}

TEST(functions, parse_min_3_4)
{
    Expression_Parser parser;
    auto result = parser.parse("min(3,4)");
    ASSERT_TRUE(result);
    EXPECT_EQ(min(3,4), result.expression->evaluate());
}

TEST(functions, parse_min_var_1_4)
{
    Expression_Parser parser;
    MemoryDataContext dc;
    int var_1 = 3;
    dc.add_or_set("var_1", var_1);
    auto result = parser.parse("min(${var_1},4)");
    ASSERT_TRUE(result);
    EXPECT_EQ(min(var_1,4), result.expression->evaluate(&dc));
    var_1=6;
    dc.add_or_set("var_1", var_1);
    EXPECT_EQ(min(var_1,4), result.expression->evaluate(&dc));
}

TEST(functions, parse_max_min_2_0_minus_1)
{
    Expression_Parser parser;
    auto result = parser.parse("max(min(2,0),-1)");
    ASSERT_TRUE(result);
    EXPECT_EQ(max(min(2,0),-1), result.expression->evaluate());
}