#pragma once

#include <string>
#include <sstream>
#include <memory>
#include "utils.hpp"
#include <cmath>


using namespace std;

class Expression
{
    public:
        virtual long evaluate() const = 0;
        virtual string to_string() const = 0;
};

class Constant_Expression: public Expression
{
    private:
        string value;
    public:
        Constant_Expression(const string& value): value(value){}
        Constant_Expression(string&& value): value(move(value)){}
        long evaluate() const override;
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
        virtual bool can_add_member() = 0;
        virtual string get_reg_id() const {return id;};
};

class Binary_Operation_Expression: public Operation_Expression
{
    protected:
    unique_ptr<Expression> left_member{nullptr};
    unique_ptr<Expression> right_member{nullptr};
    public:
        Binary_Operation_Expression(const string& id, int priority=0): Operation_Expression(id, priority){}
        bool can_add_member() override {return !(left_member && right_member);}
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
        long evaluate() const override;
        string get_reg_id() const override {return "\\"+id;}
};

class Substraction_Expression: public Binary_Operation_Expression
{
    public:
        Substraction_Expression(): Binary_Operation_Expression("-"){}
        long evaluate() const override;
};

class Multiplication_Expression: public Binary_Operation_Expression
{
    public:
        Multiplication_Expression(): Binary_Operation_Expression("*", 1){}
        long evaluate() const override;
        string get_reg_id() const override {return "\\"+id;}
};

class Division_Expression: public Binary_Operation_Expression
{
    public:
        Division_Expression(): Binary_Operation_Expression("/", 1){}
        long evaluate() const override;
};

class Exp_Expression: public Binary_Operation_Expression
{
    public:
        Exp_Expression(): Binary_Operation_Expression("^", 2){}
        long evaluate() const override;
        string get_reg_id() const override {return "\\"+id;}
};

class Modulo_Expression: public Binary_Operation_Expression
{
    public:
        Modulo_Expression(): Binary_Operation_Expression("%", 1){}
        long evaluate() const override;
};

class Equality_Expression: public Binary_Operation_Expression
{
    public:
        Equality_Expression(): Binary_Operation_Expression("==", -2){}
        long evaluate() const override;
};

class LessThan_Expression: public Binary_Operation_Expression
{
    public:
        LessThan_Expression(): Binary_Operation_Expression("<", -1){}
        long evaluate() const override;
};

class LessThanEqual_Expression: public Binary_Operation_Expression
{
    public:
        LessThanEqual_Expression(): Binary_Operation_Expression("<=", -1){}
        long evaluate() const override;
};