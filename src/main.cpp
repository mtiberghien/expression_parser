#include "parser.hpp"
#include <iostream>
#include <iterator>


using namespace std;

int main()
{
/*     Expression_Parser parser;
    cout << "Mathematical expression parser" << endl;
    cout << "Supported operators: ";
    vector<string> ops = parser.getSupportedOperators();
    for_each(begin(ops), end(ops), [i=0,size=ops.size()](const string& s) mutable {cout << s << (++i < size ? ", " : "\n");});
    cout << "Group separators: ()" << endl;
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
                long result = parse_result.expression->evaluate();
                cout << parse_result.expression->to_string() << endl;
                cout << expression << " = " << result << endl;
            }
            else
            {
                cout << "Invalid expression: " << parse_result.error_message << endl;
            }
        }
    } */

    regex r("(\\d+|\\$\\{\\w+(\\.\\w+){0,}\\})");
    string test="1+4 > ${myVariable.test} + 3";
    test = regex_replace(test, r, "<var>");
    cout << "test:" << test << endl;
}