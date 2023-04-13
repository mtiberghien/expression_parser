#include "parser.hpp"

Expression_Parser::Expression_Parser()
{
    operations.insert({"+", [](){return make_unique<Addition_Expression>();}});
    operations.insert({"-", [](){return make_unique<Substraction_Expression>();}});
    operations.insert({"*", [](){return make_unique<Multiplication_Expression>();}});
    operations.insert({"/", [](){return make_unique<Division_Expression>();}});
}

void add_member(unique_ptr<Operation_Expression>& expr, unique_ptr<Expression>& member)
{
    if(expr->can_add_member())
    {
        expr->add_member(move(member));
    }
}

const Validation_Result Expression_Parser::validate(const string& expr) noexcept
{
    Validation_Result result;
    try
    {
        // Empty or white space
        if(is_whitespace_or_empty(expr))
        {
            throw invalid_argument("Expression should not be empty");
        }

        // Unbalanced parenthesis
        int nb_open_parenthesis = count_if(begin(expr), end(expr), [](const char c){return c == '(';});
        int nb_close_parenthesis = count_if(begin(expr), end(expr), [](const char c){return c == ')';});

        if(nb_open_parenthesis != nb_close_parenthesis)
        {
            ostringstream s;
            s << "The number of opening parenthesis (" << nb_open_parenthesis << ") is not equal to the number of closing parenthesis (" << nb_close_parenthesis << ")";
            throw invalid_argument(s.str());
        }

        // Supported operators
        string supportedOperators = this->getValidOperators();
        auto invalid_it = find_if_not(begin(expr), end(expr), [supportedOperators](const char c){ return isspace(c) || isdigit(c) || (supportedOperators.find_first_of(c) != string::npos);});
        if(invalid_it != expr.end())
        {
            ostringstream message;
            message << "The following operator is not supported:  '" << *invalid_it << "'";
            throw invalid_argument(message.str());
        }

        // Expression should be separated by operators
        stringstream splitters;
        splitters << "[\\s()]+";
        regex re(splitters.str());
        sregex_token_iterator first{expr.begin(), expr.end(), re, -1}, last;
        vector<std::string> tokens{first, last};
        tokens.erase(remove_if(begin(tokens),end(tokens), is_whitespace_or_empty), end(tokens));
        if(tokens.size()>1)
        {
            vector<string> ops = getSupportedOperators();
            stringstream s_ops;
            s_ops << "[";
            for(int i=0;i<ops.size();i++)
            {
                s_ops << ops[i];
            }
            s_ops << "]";
            string r_ops = s_ops.str();
            regex end_with_operation("^[\\d" + supportedOperators+ "]*"+r_ops+"$");
            regex start_with_operation("^" + r_ops+"[\\d" + supportedOperators+ "]*$");
            bool check_operator = false;
            for(const auto& word: tokens)
            {
                if(check_operator && !regex_match(word, start_with_operation))
                {
                    throw invalid_argument("Two expressions should be separated by an operator");
                }
                check_operator = !regex_match(word, end_with_operation);
            }
            
        }
    }
    catch(const std::exception& e)
    {
        result.message = e.what();
        result.is_valid = false;
    }

    return result;
}

string Expression_Parser::getValidOperators()
{
    ostringstream s;
    s << "()";
    for(const auto& p: operations)
    {
        s << p.first;
    }

    return s.str();
}

vector<string> Expression_Parser::getSupportedOperators()
{
    vector<string> result;
    for(const auto&p: operations)
    {
        result.push_back(p.first);
    }
    return result;
}

const Parse_Result Expression_Parser::parse(istringstream& s) noexcept
{
    Parse_Result result;
    ostringstream word{""s};
    unique_ptr<Operation_Expression> current{nullptr};
    unique_ptr<Expression> member{nullptr};
    stack<unique_ptr<Operation_Expression>> ops;
    char c;
    try
    {
        while(s.get(c))
    {
        if(c==')')break;
        if(c=='(')
        {
            auto p = parse(s);
            if(p)
            {
                member = move(p.expression);
            }
            else
            {
                result.error_message = p.error_message;
                return result;
            }
            
            continue;
        }
        string id{c};
        if(operations.find(id) != operations.end())
        {
            string constant = word.str();
            if(!member)
            {
                member = make_unique<Constant_Expression>(constant);
            }
            word.str("");
            current = operations[id]();
            if(!ops.empty())
            {
                if(*ops.top() < *current)
                {
                    add_member(current, member);
                }
                else
                {
                    add_member(ops.top(), member);
                    if(*current < *ops.top())
                    {
                        while(ops.size()>1)
                        {
                            member = move(ops.top());
                            ops.pop();
                            add_member(ops.top(), member);
                        }
                    }
                    member = move(ops.top());
                    add_member(current, member);
                    ops.pop(); 
                }
            }
            else
            {
                add_member(current, member);
            }
            ops.push(move(current));
        }
        else
        {
            word << c;
        }
    }
        string constant = word.str();
        if(!member)
        {
            member = make_unique<Constant_Expression>(constant);
        }
    }
    catch(const std::exception& e)
    {
        result.error_message = e.what();
        return result;
    }
    
    
    if(!ops.empty())
    {
        add_member(ops.top(), member);   
        while(ops.size()>1)
        {
            member = move(ops.top());
            ops.pop();
            add_member(ops.top(), member);
        }
        result.expression = move(ops.top());
        ops.pop();
    }
    else
    {
        result.expression = move(member);
    }
    return result;
}

const Parse_Result Expression_Parser::parse(const string& expression_string) noexcept
{
    Validation_Result validation = validate(expression_string);
    Parse_Result result;
    if(validation)
    {
        istringstream s{expression_string};
        return parse(s);
    }
    else
    {
        result.error_message = validation.message;
    }
    return result;
}