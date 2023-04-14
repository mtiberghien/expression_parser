#include "parser.hpp"

Expression_Parser::Expression_Parser()
{
    operation_factory.insert({"+", [](){return make_unique<Addition_Expression>();}});
    operation_factory.insert({"-", [](){return make_unique<Substraction_Expression>();}});
    operation_factory.insert({"*", [](){return make_unique<Multiplication_Expression>();}});
    operation_factory.insert({"/", [](){return make_unique<Division_Expression>();}});
    operation_factory.insert({"%", [](){return make_unique<Modulo_Expression>();}});
    operation_factory.insert({"^", [](){return make_unique<Exp_Expression>();}});
    operation_factory.insert({"==", [](){return make_unique<Equality_Expression>();}});
    operation_factory.insert({"<", [](){return make_unique<LessThan_Expression>();}});
    operation_factory.insert({"<=", [](){return make_unique<LessThanEqual_Expression>();}});
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
        string supportedOperators = this->getValidOperatorsString();
        vector<string> ops = getSupportedOperatorsReg();
        sort(begin(ops), end(ops), [](const string& s1, const string& s2){return s1.size()>s2.size();});
        stringstream s_ops;
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
        string r_ops = s_ops.str();
        string digits = regex_replace(expr, regex(r_ops), "");
        auto invalid_it = find_if_not(begin(digits), end(digits), [](const char c){return isdigit(c);});
        if(invalid_it != digits.end())
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

string Expression_Parser::getValidOperatorsString()
{
    ostringstream s;
    s << "()";
    for(const auto& p: operation_factory)
    {
        s << p.first;
    }

    return s.str();
}

vector<string> Expression_Parser::getSupportedOperators()
{
    vector<string> result;
    for(const auto&p: operation_factory)
    {
        result.push_back(p.first);
    }
    return result;
}

vector<string> Expression_Parser::getSupportedOperatorsReg()
{
    vector<string> result;
    for(const auto&p: operation_factory)
    {
        result.push_back(p.second()->get_reg_id());
    }
    return result;
}

void unstack_operations(stack<unique_ptr<Operation_Expression>>& ops)
{
    unique_ptr<Expression> member{nullptr};
    while(ops.size()>1)
    {
        member = move(ops.top());
        ops.pop();
        add_member(ops.top(), member);
    }
}

bool Expression_Parser::is_operation_id(vector<string>& supported_operators, istringstream& stream, string& first_char)
{
    vector<string> match_ops;
    copy_if(begin(supported_operators), end(supported_operators), back_insert_iterator(match_ops), [&first_char](const string& id){return id[0]==first_char[0];});
    //No match means not an operator
    if(match_ops.size()==0)
    {
        return false;
    }
    // If only one match and size is one return true othewise check if following chars are matching with the id
    if(match_ops.size() == 1)
    {
        int id_size = match_ops[0].size();
        if(id_size == 1)
        {
            return true;
        }
        else
        {
            stringstream ids;
            ids << first_char;
            int i=1;
            for(i;i<id_size;i++)
            {
                char c;
                stream.get(c);
                ids << c;
            }
            string id = ids.str();
            if(id == match_ops[0])
            {
                first_char = id;
                return true;
            }
            else
            {
                stream.seekg(-(i-1), ios_base::cur);
                return false;
            }
        }
    }
    // If more than one match we should find the unique match looking to following characters
    int offset=1;
    stringstream ids;
    ids << first_char;
    do
    {
        char c;
        stream.get(c);
        ids << c;
        offset++;
        int n = count_if(begin(match_ops), end(match_ops), [&ids, offset](const string& s){ return s.substr(0,offset) == ids.str();});
        if(n >= 1)
        {
            first_char = ids.str();
            if(n==1)
            {
                return true;
            }
           
        }
        else
        {
            stream.seekg(-(offset-1), ios_base::cur);
            return true;
        }

    } while (true);
    
    return false;
}

const Parse_Result Expression_Parser::parse(istringstream& s) noexcept
{
    Parse_Result result;
    ostringstream word{""s};
    unique_ptr<Operation_Expression> current{nullptr};
    unique_ptr<Expression> member{nullptr};
    stack<unique_ptr<Operation_Expression>> ops;
    vector<string> supportedOperators = getSupportedOperators();
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
        //TODO check for id has to support ids with multiple characters like ==
        if(is_operation_id(supportedOperators, s, id))
        {
            string constant = word.str();
            if(!member)
            {
                member = make_unique<Constant_Expression>(constant);
            }
            word.str("");
            current = operation_factory[id]();
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
                        unstack_operations(ops);
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
        unstack_operations(ops);
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