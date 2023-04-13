#pragma once

#include <string>
#include <sstream>
#include <memory>
#include "utils.hpp"


using namespace std;

class Expression
{
    public:
        virtual int evaluate() const = 0;
        virtual string to_string() const = 0;
};

class Constant_Expression: public Expression
{
    private:
        string value;
    public:
        Constant_Expression(const string& value): value(value){}
        Constant_Expression(string&& value): value(move(value)){}
        int evaluate() const override;
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
        int evaluate() const override;
};

class Substraction_Expression: public Binary_Operation_Expression
{
    public:
        Substraction_Expression(): Binary_Operation_Expression("-"){}
        int evaluate() const override;
};

class Multiplication_Expression: public Binary_Operation_Expression
{
    public:
        Multiplication_Expression(): Binary_Operation_Expression("*", 1){}
        int evaluate() const override;
};

class Division_Expression: public Binary_Operation_Expression
{
    public:
        Division_Expression(): Binary_Operation_Expression("/", 1){}
        int evaluate() const override;
};