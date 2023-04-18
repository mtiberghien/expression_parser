#pragma once

#include <string>
#include <map>
#include <algorithm>

using namespace std;

class DataContext
{
    public:
        virtual bool exists(const string& reference) = 0;
        virtual long evaluate(const string& reference) = 0;
};

class MemoryDataContext: public DataContext
{
    private:
        map<string, long> values;
    public:
        bool exists(const string& reference) override { return values.find(reference)!= values.end();}
        long evaluate(const string& reference) override {return values[reference];}
        void add_or_set(const string& reference, long value)
        {
            values[reference] = value;
        }
};