#pragma once
#include <stdexcept>
using namespace std;

class System_Errors : public runtime_error
{
    public:
        string filename_err;
        System_Errors(const string& m, const string& f) : runtime_error(m), filename_err(f){}
        //const std::string& can bind lvalue and rvalue
};

class Hash_Algorithm_Error: public System_Errors
{
    public:
        using System_Errors::System_Errors;
};

class Invalid_Input: public System_Errors
{
    public:
        using System_Errors::System_Errors;
};