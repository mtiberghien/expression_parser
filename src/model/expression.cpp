#include "expression.hpp"

long Addition_Expression::evaluate() const
{
    if(left_member && right_member)
    {
        return left_member->evaluate() + right_member->evaluate();
    }
    return 0;
}

long Substraction_Expression::evaluate() const
{
    if(left_member && right_member)
    {
        return left_member->evaluate() - right_member->evaluate();
    }
    return 0;
}

long Constant_Expression::evaluate() const
{
    if(is_whitespace_or_empty(this->value)) return 0;
    return stoi(this->value);
}

long Multiplication_Expression::evaluate() const
{
    if(left_member && right_member)
    {
        return left_member->evaluate() * right_member->evaluate();
    }
    return 0;
}

long Division_Expression::evaluate() const
{
    if(left_member && right_member)
    {
        int div = right_member->evaluate();
        return left_member->evaluate() / (div == 0 ? 1: div);
    }
    return 0;
}

long Exp_Expression::evaluate() const
{
    if(left_member && right_member)
    {
        return pow(left_member->evaluate(), right_member->evaluate());
    }
    return 0;
}

long Modulo_Expression::evaluate() const
{
    if(left_member && right_member)
    {
         int div = right_member->evaluate();
        return left_member->evaluate()% (div == 0 ? 1 : div);
    }
    return 0;
}

long Equality_Expression::evaluate() const
{
    if(left_member && right_member)
    {
        return left_member->evaluate() == right_member->evaluate();
    }
    return 0;
}

long LessThan_Expression::evaluate() const
{
    if(left_member && right_member)
    {
        return left_member->evaluate() < right_member->evaluate();
    }
    return 0;
}

long LessThanEqual_Expression::evaluate() const
{
    if(left_member && right_member)
    {
        return left_member->evaluate() <= right_member->evaluate();
    }
    return 0;
}