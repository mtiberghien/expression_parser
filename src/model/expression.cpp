#include "expression.hpp"

int Addition_Expression::evaluate() const
{
    if(left_member && right_member)
    {
        return left_member->evaluate() + right_member->evaluate();
    }
    return 0;
}

int Substraction_Expression::evaluate() const
{
    if(left_member && right_member)
    {
        return left_member->evaluate() - right_member->evaluate();
    }
    return 0;
}

int Constant_Expression::evaluate() const
{
    if(is_whitespace_or_empty(this->value)) return 0;
    return stoi(this->value);
}

int Multiplication_Expression::evaluate() const
{
    if(left_member && right_member)
    {
        return left_member->evaluate() * right_member->evaluate();
    }
    return 0;
}