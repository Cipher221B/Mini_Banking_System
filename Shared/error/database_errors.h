#pragma once
#include <stdexcept>

class DataBaseErrors : public std::runtime_error //Group Error
{
    public:
        std::string filename_err;
        int line_err;
        std::string state; 
        int native_err;
        DataBaseErrors(const std::string& m, const std::string& f, int l, const std::string& s, int n) : std::runtime_error(m), filename_err(f), line_err(l), state(s), native_err(n){}
};

class ConnectionErrors : public DataBaseErrors //Member of Group
{
    public:
        using DataBaseErrors::DataBaseErrors; //Inheritance all Constructor of ConnectionErrors
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

class Data_Access_Error : public DataBaseErrors
{
    public:
        using DataBaseErrors::DataBaseErrors;
};

class Data_Error : public DataBaseErrors
{
    public:
        using DataBaseErrors::DataBaseErrors;
        
};

class Query_Error : public DataBaseErrors
{
    public:
        using DataBaseErrors::DataBaseErrors;
};
