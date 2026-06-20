#include "account.h"
#include <stdexcept>
#include "security/random_generation.h"
#include "Shared/error/system_errors.h"
#include "utils/input_validation.h"
using namespace std;

//setter
void Account::set_account_id(int i)
{
    if(!Input_Validation::check_id(i))
    {
        throw Invalid_Input("Account ID is Invalid.", __FILE__, __LINE__);
    }
    account_id = i;
}

void Account::set_account_no(const string& n)
{
    if(!Input_Validation::check_account_no(n))
    {
        throw Invalid_Input("Account NO is Invalid.", __FILE__, __LINE__);
    }
    account_no = n;
}

void Account::set_user_id(int i)
{
    if(!Input_Validation::check_id(i))
    {
        throw Invalid_Input("User ID is Invalid.", __FILE__, __LINE__);
    }
    user_id = i;
}

void Account::set_balance(double b)
{
    if(!Input_Validation::check_balance(b))
    {
        throw Invalid_Input("Invalid Balance", __FILE__, __LINE__);
    }
    balance = b;
}

void Account::set_type(const string& t)
{
    if(!Input_Validation::check_type_account(t))
    {
        throw Invalid_Input("Type Account is Invalid.", __FILE__, __LINE__);
    }
    type = t;
}

void Account::set_status(const string& s)
{
    if(!Input_Validation::check_status(s))
    {
        
    }
    status = s;
}

void Account::make_type(string t)
{
    if(!Input_Validation::check_user_input(t))
    {
        throw Invalid_Input("Type Account is Invalid.", __FILE__, __LINE__);
    }
    switch(stoi(t))
    {
        case 1:
        {
            type = "CURRENT";
            break;

        }
        case 2:
        {
            type = "BUSINESS";
            break;

        }
        default:
        {
            throw Invalid_Input("Invalid Input.", __FILE__, __LINE__);
        }
    }

}

void Account::clear_sensitive_data()
{
    status.clear();
}

void Account::clear_all_account_data()
{
    account_id = 0;
    user_id = 0;
    account_no.clear();
    balance = 0;
    type.clear();
    clear_sensitive_data();
}