#pragma once

#include <string>
#include <map>
#include <algorithm>
#include <vector>

using namespace std;

class DataContext
{
    public:
        virtual bool exists(const string& reference) const = 0;
        virtual long evaluate(const string& reference) const = 0;
        virtual vector<string> get_keys() const = 0;
};

class MemoryDataContext: public DataContext
{
    private:
        map<string, long> values;
    public:
        bool exists(const string& reference) const override { return values.find(reference)!= values.end();}
        long evaluate(const string& reference) const override {return values.at(reference);}
        vector<string> get_keys() const override{
            vector<string> result;
            for(const auto& p: values)
            {
                result.push_back(p.first);
            }
            return result;
        }
        void add_or_set(const string& reference, long value)
        {
            values[reference] = value;
        }
        operator map<string,long>() const {return values;}
};