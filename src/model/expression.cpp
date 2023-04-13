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

int Division_Expression::evaluate() const
{
    if(left_member && right_member)
    {
        int div = right_member->evaluate();
        return left_member->evaluate() / (div == 0 ? 1: div);
    }
    return 0;
}

int Exp_Expression::evaluate() const
{
    if(left_member && right_member)
    {
        return pow(left_member->evaluate(), right_member->evaluate());
    }
    return 0;
}

int Modulo_Expression::evaluate() const
{
    if(left_member && right_member)
    {
         int div = right_member->evaluate();
        return left_member->evaluate()% (div == 0 ? 1 : div);
    }
    return 0;
}