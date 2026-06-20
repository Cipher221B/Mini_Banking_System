#pragma once
#include <stdexcept>

class System_Errors : public std::runtime_error
{
    public:
        std::string filename_err;
        int line_err;
        System_Errors(const std::string& m, const std::string& f, int l) : runtime_error(m), filename_err(f), line_err(l){}
};

class Invalid_Input: public System_Errors
{
    public:
        using System_Errors::System_Errors;
};

class Open_File_Error: public System_Errors
{
    public:
        using System_Errors::System_Errors;

};