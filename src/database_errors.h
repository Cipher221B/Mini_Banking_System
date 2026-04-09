#pragma once
#include <stdexcept>
using namespace std;

class DataBaseErrors : public runtime_error //Group Error
{
    public:
        string filename_err;
        string state; 
        int native_err;
        DataBaseErrors(const string& m, const string& f, const string& s, int n) : runtime_error(m), filename_err(f), state(s), native_err(n){}
        //const std::string& can bind lvalue and rvalue
};


class ConnectionErrors : public DataBaseErrors //Member of Group
{
    public:
        using DataBaseErrors::DataBaseErrors; //Inheritance all Constructor of ConnectionErrors
};

class Create_New_User_Error : public DataBaseErrors //Member of Group
{
    public:
        using DataBaseErrors::DataBaseErrors;
        
};

class Create_New_Account_Error : public DataBaseErrors //Member of Group
{
    public:
        using DataBaseErrors::DataBaseErrors;

};

class Create_Log_Error : public DataBaseErrors
{
    public:
        using DataBaseErrors::DataBaseErrors;
};

class Get_Data_Error : public DataBaseErrors
{
    public:
        using DataBaseErrors::DataBaseErrors;
};

class Get_User_Error : public DataBaseErrors
{
    public:
        using DataBaseErrors::DataBaseErrors;
};

class Get_Account_Error : public DataBaseErrors
{
    public:
        using DataBaseErrors::DataBaseErrors;
};


class Data_Error : public DataBaseErrors
{
    public:
        using DataBaseErrors::DataBaseErrors;
        
};

class Transaction_Error : public DataBaseErrors
{
    public:
        using DataBaseErrors::DataBaseErrors;
};

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
