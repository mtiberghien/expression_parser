#include <gtest/gtest.h>
#include "parser.hpp"


TEST(multiplication, parse_const_3_mult_4)
{
    Expression_Parser parser;
    EXPECT_EQ(3*4, parser.parse("3*4").expression->evaluate());
}