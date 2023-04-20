#include "expression.hpp"

long Addition_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) + right_member->evaluate(dc);
    }
    return 0;
}

long Substraction_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) - right_member->evaluate(dc);
    }
    return 0;
}

long Constant_Expression::evaluate(const DataContext* dc) const
{
    if(is_whitespace_or_empty(this->value)) return 0;
    return stol(this->value);
}

long Multiplication_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) * right_member->evaluate(dc);
    }
    return 0;
}

long Division_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        int div = right_member->evaluate(dc);
        return left_member->evaluate(dc) / (div == 0 ? 1: div);
    }
    return 0;
}

long Exp_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return pow(left_member->evaluate(dc), right_member->evaluate(dc));
    }
    return 0;
}

long Modulo_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
         int div = right_member->evaluate(dc);
        return left_member->evaluate(dc)% (div == 0 ? 1 : div);
    }
    return 0;
}

long Equals_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) == right_member->evaluate(dc);
    }
    return 0;
}

long NotEquals_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) != right_member->evaluate(dc);
    }
    return 0;
}

long LessThan_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) < right_member->evaluate(dc);
    }
    return 0;
}

long LessThanEqual_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) <= right_member->evaluate(dc);
    }
    return 0;
}

long GreaterThan_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) > right_member->evaluate(dc);
    }
    return 0;
}

long GreaterThanEqual_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) >= right_member->evaluate(dc);
    }
    return 0;
}

long And_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) && right_member->evaluate(dc);
    }
    return 0;
}

long Or_Expression::evaluate(const DataContext* dc)const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) || right_member->evaluate(dc);
    }
    return 0;
}

long Reference_Expression::evaluate(const DataContext* dc) const
{
    if(dc->exists(reference))
    {
        return dc->evaluate(reference);
    }
    return 0;
}

long Not_Expression::evaluate(const DataContext* dc) const
{
    if(right_member)
    {
        return !right_member->evaluate(dc);
    }
    return 0;
}

long Shift_Left_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) << right_member->evaluate(dc);
    }
    return 0;
}

long Shift_Right_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) >> right_member->evaluate(dc);
    }
    return 0;
}

long Max_Function_Expression::evaluate(const DataContext* dc) const
{
    if(args.size() >= 2)
    {
       long arg1 = args[0]->evaluate(dc);
       long arg2 = args[1]->evaluate(dc);
       return max(arg1, arg2); 
    }
    else if(args.size() == 1)
    {
        return args[1]->evaluate();
    }
    return 0;
}