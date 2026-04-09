#pragma once 

#include <iostream>
using namespace std;
class Support
{
    public:
        static string standardization_number(int value, int n, char c);
        static string get_query(const string& filename);
};