#pragma once

#include <iostream>

class System_Log
{
    public:
        static void error_log(const std::string& type_err, const std::string& namefile, const std::string& namefile_err, const int& line_error, const std::string& state, int native, const std::string& message); //catch detail database error
        static void error_log(const std::string& type_err, const std::string& namefile, const std::string& namefile_err, const int& line_error, const std::string& message); //catch error of Audit Log
        static std::string get_real_time();

};