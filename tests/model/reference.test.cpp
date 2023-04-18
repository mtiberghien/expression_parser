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


TEST(reference, parse_ref_with_update)
{
    MemoryDataContext mdc;
    mdc.add_or_set("reference_1", 10);
    Expression_Parser parser;
    auto result = parser.parse("${reference_1} + 3");
    ASSERT_TRUE(result);
    EXPECT_EQ(13, result.expression->evaluate(&mdc));
    mdc.add_or_set("reference_1", 1);
    EXPECT_EQ(4, result.expression->evaluate(&mdc));
}

TEST(reference, parse_ref_equals_with_update)
{
    MemoryDataContext mdc;
    mdc.add_or_set("capteur.presence", 0);
    Expression_Parser parser;
    auto result = parser.parse("${capteur.presence} == 1");
    ASSERT_TRUE(result);
    EXPECT_FALSE(result.expression->evaluate(&mdc));
    mdc.add_or_set("capteur.presence", 1);
    EXPECT_TRUE(result.expression->evaluate(&mdc));
}

TEST(reference, valid_references)
{
    Expression_Parser parser;
    string wrong_refs[]= {"${a}", "${a.b.c}", "${test_1}", "${a_1.b_2}"};
    for(const auto& s: wrong_refs)
    {
        EXPECT_TRUE(parser.parse(s));
    }
    
}

