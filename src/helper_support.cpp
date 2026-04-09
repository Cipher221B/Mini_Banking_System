#include "helper_support.h"
#include <sstream>
#include <iomanip>
#include <fstream>
#include "system_log.h"

string Support::standardization_number(int value, int n, char c)
{
    stringstream ss;
    ss << setw(n) << setfill(c) << to_string(value);
    return ss.str();
}

string Support::get_query(const string& filename)
{
    string temp_query, query;
    ifstream read(filename);

    if(read.is_open())
    {
        while(getline(read, temp_query))
        {
            query += temp_query + '\n';

        }
    }
    else
    {
        cerr << "Error!!! Can't Open File Name: " << "' << filename << '. Read File Failed!!!\n";
    }
    read.close();
    return query;
}

