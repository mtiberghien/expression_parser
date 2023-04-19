#include "parser.hpp"

Expression_Parser::Expression_Parser()
{
    operation_factory.insert({"+", [](){return make_unique<Addition_Expression>();}});
    operation_factory.insert({"-", [](){return make_unique<Substraction_Expression>();}});
    operation_factory.insert({"*", [](){return make_unique<Multiplication_Expression>();}});
    operation_factory.insert({"/", [](){return make_unique<Division_Expression>();}});
    operation_factory.insert({"%", [](){return make_unique<Modulo_Expression>();}});
    operation_factory.insert({"^", [](){return make_unique<Exp_Expression>();}});
    operation_factory.insert({"==", [](){return make_unique<Equals_Expression>();}});
    operation_factory.insert({"<", [](){return make_unique<LessThan_Expression>();}});
    operation_factory.insert({"<=", [](){return make_unique<LessThanEqual_Expression>();}});
    operation_factory.insert({">", [](){return make_unique<GreaterThan_Expression>();}});
    operation_factory.insert({">=", [](){return make_unique<GreaterThanEqual_Expression>();}});
    operation_factory.insert({"!=", [](){return make_unique<NotEquals_Expression>();}});
    operation_factory.insert({"and", [](){return make_unique<And_Expression>();}});
    operation_factory.insert({"or", [](){return make_unique<Or_Expression>();}});
    operation_factory.insert({"!", [](){return make_unique<Not_Expression>();}});
}

void add_member(unique_ptr<Operation_Expression>& expr, unique_ptr<Expression>& member, bool is_left_member)
{
    if(expr->can_add_member() && (!is_left_member || expr->has_left_member()))
    {
        expr->add_member(move(member));
    }
    else
    {
        member = nullptr;
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

        
        int openingIndex = -1;
        string substr = expr;
        while(nb_close_parenthesis > 0)
        {
            auto closing_it=substr.begin();
            int last_closing_index = substr.find_last_of(')');
            advance(closing_it, last_closing_index);
            nb_open_parenthesis = count(substr.begin(),closing_it,'(');
            if (nb_open_parenthesis<nb_close_parenthesis)
            {
                throw invalid_argument("The closing parenthesis cannot be before the opening parenthesis");
            }
            substr = substr.substr(0,last_closing_index);
            nb_close_parenthesis--;
        }



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
        string digits = regex_replace(expr, regex(r_ops, regex_constants::icase), "");
        regex is_ref("\\$\\{\\w+(\\.\\w+){0,}\\}");
        digits = regex_replace(digits, is_ref, "1");
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
            regex end_with_operation("^[\\d" + r_ops+ "]*"+r_ops+"$", regex_constants::icase);
            regex start_with_operation("^" + r_ops+"[\\d" + r_ops+ "]*$", regex_constants::icase);
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
    }
    catch(const std::exception& e)
    {
        result.message = e.what();
        result.is_valid = false;
    }

    return result;
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
        add_member(ops.top(), member, false);
    }
}

char safe_to_lower(char c)
{
    return static_cast<char>(tolower(static_cast<unsigned char>(c)));
}

bool Expression_Parser::is_operation_id(vector<string>& supported_operators, istringstream& stream, string& id)
{
    vector<string> match_ops;
    copy_if(begin(supported_operators), end(supported_operators), back_insert_iterator(match_ops), [&id](const string& op_id){return safe_to_lower(op_id[0])==safe_to_lower(id[0]);});
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
            ids << safe_to_lower(id[0]);
            int i=1;
            for(i;i<id_size;i++)
            {
                char c;
                stream.get(c);
                ids << safe_to_lower(c);
            }
            string complete_id = ids.str();
            if(complete_id == match_ops[0])
            {
                id = complete_id;
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
    ids << id;
    do
    {
        char c;
        stream.get(c);
        ids << c;
        offset++;
        int n = count_if(begin(match_ops), end(match_ops), [&ids, offset](const string& s){ return s.substr(0,offset) == ids.str();});
        if(n >= 1)
        {
            id = ids.str();
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

bool is_reference_id(istringstream& s, string& id)
{
    if(id[0]=='$')
    {
        ostringstream ref;
        char c;
        s.get(c);
        do
        {
            s.get(c);
            if(c!='}')
            {
                ref << c;
            }

        } while (c!='}');
        id = ref.str();
        return true;
    }
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
            if(isspace(c)) continue;
            // Return if end of group
            if(c==')')break;
            string id{c};
            // Parse recursively if start of group
            if(c=='(')
            {
                auto p = parse(s);
                // If conversion suceed store as a member
                if(p)
                {
                    member = move(p.expression);
                }
                // Else return error message
                else
                {
                    result.error_message = p.error_message;
                    return result;
                }
                // When a group is parsed we just need to read the next character
                continue;
            }
            else if(is_reference_id(s, id))
            {
                member = make_unique<Reference_Expression>(id);
                continue;
            }
            // Check if the character is an operator or the first character of an operator
            // Note that the id will contain the full id of the operation if succeed
            if(is_operation_id(supportedOperators, s, id))
            {
                // If member is not defined make member as constant expression
                string constant = word.str();
                if(!member)
                {
                    member = make_unique<Constant_Expression>(constant);
                }
                // Word is cleared
                word.str("");
                //Create the operation expression according to the id
                current = operation_factory[id]();
                //If there is another operation in the stack
                if(!ops.empty())
                {
                    //If new operator has a greater priority than previous
                    if(*ops.top() < *current)
                    {
                        // Member is added to the current operator
                        add_member(current, member, true);
                    }
                    else
                    {
                        // Member is added  to previous operator
                        add_member(ops.top(), member, false);
                        // If new operator has lower priority previous members can be merged
                        if(*current < *ops.top())
                        {
                            //All operations are merged from right to left
                            //There oldest operation will remain in the stack
                            unstack_operations(ops);
                        }
                        // There is one (after unstack) or more operations
                        member = move(ops.top());
                        // Previous operation is merged to the current
                        add_member(current, member, true);
                        ops.pop(); 
                    }
                }
                else
                {
                    // Member is added to current operation
                    add_member(current, member, true);
                }
                // Current operation is added to the stack
                ops.push(move(current));
            }
            else
            {
                // If c is not an operato the word stream accumulates it
                word << c;
            }
        }
        // At the end of the stream one last member shoud remain.
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
    
    // If the stack of operation is not empty, remainins operations should be merged from right to left
    if(!ops.empty())
    {
        //Last member is added to last operation
        add_member(ops.top(), member, false);
        // Previous operations are merged   
        unstack_operations(ops);
        // The result contains the top operation
        result.expression = move(ops.top());
        ops.pop();
    }
    else
    {
        //In case no operators the expression is a group or a constant
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