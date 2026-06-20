#pragma once
#include <stdexcept>

class BussinessErrors : public std::runtime_error //Group Error
{
    public:
        std::string filename_err;
        int line_err;
        std::string state; 
        int native_err;
        BussinessErrors(const std::string& m, const std::string& f, int l, const std::string& s, int n) : std::runtime_error(m), filename_err(f), line_err(l), state(s), native_err(n){}
        BussinessErrors(const std::string& m, const std::string& f, int l) : std::runtime_error(m), filename_err(f), line_err(l){}
};

class Create_New_User_Error : public BussinessErrors //Member of Group
{
    public:
        using BussinessErrors::BussinessErrors;
        
};

class Create_New_Account_Error : public BussinessErrors //Member of Group
{
    public:
        using BussinessErrors::BussinessErrors;

};

class Transaction_Error : public BussinessErrors
{
    public:
        using BussinessErrors::BussinessErrors;
};

class Change_Status_User_Error : public BussinessErrors
{
    public:
        using BussinessErrors::BussinessErrors;
};

class Change_Status_Account_Error : public BussinessErrors
{
    public:
        using BussinessErrors::BussinessErrors;
};

class Change_Password_Error : public BussinessErrors
{
    public:
        using BussinessErrors::BussinessErrors;
};

class Hash_Algorithm_Error: public BussinessErrors
{
    public:
        using BussinessErrors::BussinessErrors;
};