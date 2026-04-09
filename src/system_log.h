#pragma once

#include <iostream>
using namespace std;

class System_Log
{
    public:
        static void error_log(const string& type_err, const string& namefile_err, const string& state, int native, const string& message); //catch detail database error
        static void error_log(const string& type_err, const string& namefile_err, const string& message); //catch error of Audit Log
        static string get_real_time();

};