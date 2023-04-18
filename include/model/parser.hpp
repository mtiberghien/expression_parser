#pragma once

#include "expression.hpp"
#include <string>
#include <stack>
#include <sstream>
#include <map>
#include <memory>
#include <functional>
#include <regex>
#include <iostream>
#include <queue>
#include "data_context.hpp"

using namespace std;

struct Validation_Result
{
    bool is_valid{true};
    string message{""};
    operator bool() const {return is_valid;}
};

struct Parse_Result
{
    unique_ptr<Expression> expression{nullptr};
    string error_message{""};
    operator bool() const {if(expression){return true;} return false;};
};

class Expression_Parser
{
    private:
        unique_ptr<DataContext> data;
        map<string, function<unique_ptr<Operation_Expression>()>> operation_factory;
        const Validation_Result validate(const string& expression) noexcept;
        string getValidOperatorsString();
        bool is_operation_id(vector<string>& supported_operators, istringstream& stream, string& first_char);
        const Parse_Result parse(istringstream& stream) noexcept;
        vector<string> getSupportedOperatorsReg();
    public:
        Expression_Parser(unique_ptr<DataContext> data=nullptr);
        vector<string> getSupportedOperators();
        const Parse_Result parse(const string& expression_string) noexcept;
};