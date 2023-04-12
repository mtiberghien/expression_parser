#pragma once

#include "expression.hpp"
#include <string>
#include <stack>
#include <sstream>
#include <map>
#include <memory>
#include <functional>
#include <regex>

using namespace std;

struct Validation_Result
{
    bool is_valid{true};
    string message{""};
    operator bool(){return is_valid;}
};

struct Parse_Result
{
    unique_ptr<Expression> expression{nullptr};
    string error_message{""};
    operator bool(){if(expression){return true;} return false;};
};

class Expression_Parser
{
    private:
        map<string, function<unique_ptr<Operation_Expression>()>> operations;
        const Validation_Result validate(const string& expression) noexcept;
        string getValidOperators();
        const Parse_Result parse(istringstream& stream) noexcept;
    public:
        Expression_Parser();
        vector<string> getSupportedOperators();
        const Parse_Result parse(const string& expression_string) noexcept;
};