#include "parser.hpp"
#include <iostream>
#include <iterator>


using namespace std;

int main()
{
    Expression_Parser parser;
    cout << "Mathematical expression parser" << endl;
    cout << "Supported opérators: ";
    vector<string> ops = parser.getSupportedOperators();
    for_each(begin(ops), end(ops), [i=0,size=ops.size()](const string& s) mutable {cout << s << (++i < size ? ", " : "\n");});
    string expression;
    while(expression!="q")
    {
        cout << "Your expression ('q' to quit): ";
        getline(cin, expression);
        if(expression!="q")
        {
            auto parse_result = parser.parse(expression);
            if(parse_result)
            {
                int result = parse_result.expression->evaluate();
                cout << parse_result.expression->to_string() << endl;
                cout << expression << " = " << result << endl;
            }
            else
            {
                cout << "Invalid expression: " << parse_result.error_message << endl;
            }
        }
    }
}