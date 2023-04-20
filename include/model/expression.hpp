#pragma once

#include <string>
#include <sstream>
#include <memory>
#include "utils.hpp"
#include <cmath>
#include "data_context.hpp"
#include <vector>

using namespace std;

class Expression
{
    public:
        virtual long evaluate(const DataContext* dc = nullptr) const = 0;
        virtual string to_string() const = 0;
};

class Constant_Expression: public Expression
{
    private:
        string value;
    public:
        Constant_Expression(const string& value): value(value){}
        Constant_Expression(string&& value): value(move(value)){}
        long evaluate(const DataContext* dc) const override;
        string to_string() const override {return value;}
};

class Operation_Expression: public Expression
{
    protected:
        string id;
        int priority{0};
    public:
        Operation_Expression(const string& id, int priority=0): id(id), priority(priority){}
        bool operator <(const Operation_Expression& other){return this->priority < other.priority;}
        bool operator ==(const Operation_Expression& other){return this->priority == other.priority;}
        virtual void add_member(unique_ptr<Expression>&& expr) = 0;
        virtual bool can_add_member() const = 0;
        virtual bool has_left_member() const = 0;
        virtual string get_reg_id() const {return id;};
};


class Unary_Operation_Expression: public Operation_Expression
{
    protected:
    unique_ptr<Expression> right_member{nullptr};
    public:
        Unary_Operation_Expression(const string& id, int priority=0): Operation_Expression(id, priority){}
        bool can_add_member() const override {return !right_member;}
        bool has_left_member() const override {return false;}
        void add_member(unique_ptr<Expression>&& expr) override {
            right_member = move(expr);
        }
        string to_string() const override {return id +"(" + right_member->to_string() + ")";} 

};

class Binary_Operation_Expression: public Operation_Expression
{
    protected:
    unique_ptr<Expression> left_member{nullptr};
    unique_ptr<Expression> right_member{nullptr};
    public:
        Binary_Operation_Expression(const string& id, int priority=0): Operation_Expression(id, priority){}
        bool can_add_member() const override {return !(left_member && right_member);}
        bool has_left_member() const override {return true;}
        void add_member(unique_ptr<Expression>&& expr) override {
            if(left_member)
            {
                right_member = move(expr);
            }
            else
            {
                left_member = move(expr);
            }
        }
        string to_string() const override {return id +"(" + left_member->to_string() + ", " + right_member->to_string() + ")";} 

};

class Addition_Expression: public Binary_Operation_Expression
{
    public:
        Addition_Expression(): Binary_Operation_Expression("+"){}
        long evaluate(const DataContext* dc) const override;
        string get_reg_id() const override {return "\\"+id;}
};

class Substraction_Expression: public Binary_Operation_Expression
{
    public:
        Substraction_Expression(): Binary_Operation_Expression("-"){}
        long evaluate(const DataContext* dc) const override;
};

class Multiplication_Expression: public Binary_Operation_Expression
{
    public:
        Multiplication_Expression(): Binary_Operation_Expression("*", 1){}
        long evaluate(const DataContext* dc) const override;
        string get_reg_id() const override {return "\\"+id;}
};

class Division_Expression: public Binary_Operation_Expression
{
    public:
        Division_Expression(): Binary_Operation_Expression("/", 1){}
        long evaluate(const DataContext* dc) const override;
};

class Exp_Expression: public Binary_Operation_Expression
{
    public:
        Exp_Expression(): Binary_Operation_Expression("^", 2){}
        long evaluate(const DataContext* dc) const override;
        string get_reg_id() const override {return "\\"+id;}
};

class Modulo_Expression: public Binary_Operation_Expression
{
    public:
        Modulo_Expression(): Binary_Operation_Expression("%", 1){}
        long evaluate(const DataContext* dc) const override;
};

class Equals_Expression: public Binary_Operation_Expression
{
    public:
        Equals_Expression(): Binary_Operation_Expression("==", -3){}
        long evaluate(const DataContext* dc) const override;
};

class NotEquals_Expression: public Binary_Operation_Expression
{
    public:
        NotEquals_Expression(): Binary_Operation_Expression("!=", -3){}
        long evaluate(const DataContext* dc) const override;
};

class LessThan_Expression: public Binary_Operation_Expression
{
    public:
        LessThan_Expression(): Binary_Operation_Expression("<", -2){}
        long evaluate(const DataContext* dc) const override;
};

class LessThanEqual_Expression: public Binary_Operation_Expression
{
    public:
        LessThanEqual_Expression(): Binary_Operation_Expression("<=", -2){}
        long evaluate(const DataContext* dc) const override;
};

class GreaterThan_Expression: public Binary_Operation_Expression
{
    public:
        GreaterThan_Expression(): Binary_Operation_Expression(">", -2){}
        long evaluate(const DataContext* dc) const override;
};

class GreaterThanEqual_Expression: public Binary_Operation_Expression
{
    public:
        GreaterThanEqual_Expression(): Binary_Operation_Expression(">=", -2){}
        long evaluate(const DataContext* dc) const override;
};

class And_Expression: public Binary_Operation_Expression
{
    public:
        And_Expression(): Binary_Operation_Expression("and", -4){}
        long evaluate(const DataContext* dc) const override;
};

class Or_Expression: public Binary_Operation_Expression
{
    public:
        Or_Expression(): Binary_Operation_Expression("or", -3){}
        long evaluate(const DataContext* dc) const override;
};

class Reference_Expression: public Expression
{
    private:
        string reference;
    public:
        Reference_Expression(const string& reference):reference(reference){};
        long evaluate(const DataContext* dc) const override;
        string to_string() const override {return "ref(" + reference +")";}
};

class Not_Expression: public Unary_Operation_Expression
{
    public:
        Not_Expression(): Unary_Operation_Expression("!", 3){}
        long evaluate(const DataContext* dc) const override;
};

class Shift_Left_Expression: public Binary_Operation_Expression
{
    public:
        Shift_Left_Expression(): Binary_Operation_Expression("<<", -1){}
        long evaluate(const DataContext* dc) const override;
};

class Shift_Right_Expression: public Binary_Operation_Expression
{
    public:
        Shift_Right_Expression(): Binary_Operation_Expression(">>", -1){}
        long evaluate(const DataContext* dc) const override;
};

class Function_Expression: public Expression
{
    protected:
        string id;
        uint8_t nb_args{0};
        vector<unique_ptr<Expression>> args;
        bool can_add_arg() const {return static_cast<uint8_t>(args.size()) < nb_args;}
    public:
        Function_Expression(const string& id, uint8_t nb_args): id(id), nb_args(nb_args){};
        uint8_t getNbArgs() const {return nb_args;}
        void add_arg(unique_ptr<Expression>&& expr){
            if(can_add_arg())
            {
                args.push_back(move(expr));
            }
        }

        string to_string() const override 
        { 
            ostringstream s;
            s << id << "(";
            for(int i=0;i<args.size();i++)
            {
                s << args[i]->to_string();
                if(i<args.size()-1)
                {
                    s << ",";
                }
            }
            s << ")";
            return s.str();
        }
};

class Max_Function_Expression: public Function_Expression
{
    public:
        Max_Function_Expression(): Function_Expression("max", 2){}
        long evaluate(const DataContext* dc) const override;

};

class Min_Function_Expression: public Function_Expression
{
    public:
        Min_Function_Expression(): Function_Expression("min", 2){}
        long evaluate(const DataContext* dc) const override;

};
