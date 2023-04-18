#include "parser.hpp"
#include <iostream>
#include <iterator>
#include <random>

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

    Expression_Parser parser;
    MemoryDataContext mdc;
    mt19937 mt;
    uniform_int_distribution<int> dist(0,1);
    Parse_Result p_result = parser.parse("${presence} == 1");
    if(p_result)
    {
        int index = 0;
        while(index++ < 10)
        {
            mdc.add_or_set("presence", dist(mt));
            cout << boolalpha;
            cout << "Présence détectée: " << (bool)p_result.expression->evaluate(&mdc) << endl;
        }
    }
}