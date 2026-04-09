#include "manager_account.h"
#include "random_generation.h"
#include <stdexcept>
#include "system_errors.h"

//setter
void Account::set_account_id(int i)
{
    account_id = i;
}

void Account::set_account_no(const string& n)
{
    account_no = n;
}

void Account::set_user_id(int i)
{
    user_id = i;
}

void Account::set_balance(double b)
{
    if(b>=0)
    {
        balance = b;
    }
    throw Invalid_Input("Invalid Balance", __FILE__);

}

void Account::set_type(const string& t)
{
    type = t;
}

void Account::set_status(const string& s)
{
    status = s;
}

void Account::make_type(int c)
{
    switch(c)
    {
        case 1:
        {
            type = "CURRENT";
            break;

        }
        case 2:
        {
            type = "SAVING";
            break;

        }
        case 3:
        {
            type = "CREDIT";
            break;

        }
        case 4:
        {
            type = "BUSINESS";
            break;

        }
        default:
        {
            throw Invalid_Input("Invalid Input.", __FILE__);
        }
    }

}

