#include <gtest/gtest.h>
#include "parser.hpp"

TEST(reference, parse_simple_ref)
{
    MemoryDataContext mdc;
    mdc.add_or_set("reference_1", 10);
    Expression_Parser parser;
    auto result = parser.parse("${reference_1}");
    ASSERT_TRUE(result);
    EXPECT_EQ(10, result.expression->evaluate(&mdc));
}