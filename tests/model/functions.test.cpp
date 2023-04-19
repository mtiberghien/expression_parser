#include <gtest/gtest.h>
#include "parser.hpp"

TEST(functions, parse_max_3_4)
{
    Expression_Parser parser;
    auto result = parser.parse("max(3,4)");
    ASSERT_TRUE(result);
    EXPECT_EQ(max(3,4), result.expression->evaluate());
}