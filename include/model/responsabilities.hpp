#pragma once

#include <string>
#include <stack>
#include <sstream>
#include <map>
#include <regex>
#include "parser.hpp"


using namespace std;

class Handler
{
    public:
        virtual const Validation_Result validate(const string& expr)=0;
        virtual Handler *setNext(Handler *handler)=0;
};

class AbstractHandler: public Handler
{
    private:
        Handler *next_handler;
        
    protected:
        

    public:
        Validation_Result result;
        AbstractHandler() : next_handler(nullptr) {}
        Handler *setNext(Handler *handler) override
        {
            this->next_handler = handler;
            return handler;
        }

        const Validation_Result validate(const string& expr) override
        {
            
            if (this->next_handler)
            {
                return this->next_handler->validate(expr);
            }

            return {};
        }
        
};

class EmptyHandler : public AbstractHandler
{
    public:
        const Validation_Result validate(const string& expr) override
        {

            //Si expr est vide alors
            // result.message = "Express..."
            // result.is_valid = true;
            // return result
            // Sinon
            // return AbstractHandler::validate(expr)
            
                // Empty or white space
                if(is_whitespace_or_empty(expr))
                {
                    throw invalid_argument("Expression should not be empty");
                }
           
            
           
            return result;
            return AbstractHandler::validate(expr);
        }
};

class ParenthesisHandler : public AbstractHandler
{
    public:
        const Validation_Result validate(const string& expr) override
        {
             // Unbalanced parenthesis
            
                nb_open_parenthesis = count_if(begin(expr), end(expr), [](const char c){return c == '(';});
                nb_close_parenthesis = count_if(begin(expr), end(expr), [](const char c){return c == ')';});
                if(nb_open_parenthesis != nb_close_parenthesis)
                {  
                    s << "The number of opening parenthesis (" << nb_open_parenthesis << ") is not equal to the number of closing parenthesis (" << nb_close_parenthesis << ")";
                    throw invalid_argument(s.str());
                }

                // Parenthesis order
                substr = expr;
                while(nb_close_parenthesis > 0)
                {
                    auto closing_it=substr.begin();
                    last_closing_index = substr.find_last_of(')');
                    advance(closing_it, last_closing_index);
                    nb_open_parenthesis = count(substr.begin(),closing_it,'(');
                    if (nb_open_parenthesis<nb_close_parenthesis)
                    {
                        throw invalid_argument("The closing parenthesis cannot be before the opening parenthesis");
                    }
                    substr = substr.substr(0,last_closing_index);
                    nb_close_parenthesis--;
                }


            
            return AbstractHandler::validate(expr);
        
        }

    private:

        int nb_open_parenthesis;
        int nb_close_parenthesis;
        ostringstream s;
        int openingIndex = -1;
        string substr;
        int last_closing_index;

};


class InvalidoperatorHandler : public AbstractHandler
{
    public:
        const Validation_Result validate(const string& expr) override
        {
            
                ops = getSupportedOperatorsReg();
                sort(begin(ops), end(ops), [](const string& s1, const string& s2){return s1.size()>s2.size();});
                
                s_ops << "(\\s|\\(|\\)";
                if(ops.size()>0)
                {
                    s_ops << "|";
                }
                for(int i=0;i<ops.size();i++)
                {
                    s_ops << ops[i];
                    if(i<ops.size()-1)
                    {
                        s_ops << "|";
                    }
                }
                s_ops << ")";
                r_ops = s_ops.str();
                digits = regex_replace(expr, regex(r_ops, regex_constants::icase), "");
                is_ref("\\$\\{\\w+(\\.\\w+){0,}\\}");
                digits = regex_replace(digits, is_ref, "1");
                auto invalid_it = find_if_not(begin(digits), end(digits), [](const char c){return isdigit(c);});
                if(invalid_it != digits.end())
                {
                    message << "The following operator is not supported:  '" << *invalid_it << "'";
                    throw invalid_argument(message.str());
                }

            
           
            
            return AbstractHandler::validate(expr);
            
        }

    private:
        vector<string> ops;
        string r_ops;
        string digits;
        regex is_ref;
        ostringstream message; 
        stringstream s_ops;
};

class NooperatordHandler : public AbstractHandler
{
    public:
        const Validation_Result validate(const string& expr) override
        {
            
                // Expression should be separated by operators
                
                splitters << "[\\s()]+";
                re(splitters.str());
                first{expr.begin(), expr.end(), re, -1}, last;
                tokens{first, last};
                tokens.erase(remove_if(begin(tokens),end(tokens), is_whitespace_or_empty), end(tokens));
                if(tokens.size()>1)
                {
                    end_with_operation("^[\\d" + r_ops+ "]*"+r_ops+"$", regex_constants::icase);
                    start_with_operation("^" + r_ops+"[\\d" + r_ops+ "]*$", regex_constants::icase);
                    bool check_operator = false;
                    for(auto& word: tokens)
                    {
                        word = regex_replace(word, is_ref, "1");
                        if(check_operator && !regex_match(word, start_with_operation))
                        {
                            throw invalid_argument("Two expressions should be separated by an operator");
                        }
                        check_operator = !regex_match(word, end_with_operation);
                    }
                    
                }
           
            return result;
            return AbstractHandler::validate(expr);
        }

    private:
        stringstream splitters;
        vector<std::string> tokens;
        regex re;
        sregex_token_iterator first;
        regex end_with_operation;
        regex start_with_operation;



};

