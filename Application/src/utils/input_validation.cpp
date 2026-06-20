#include "input_validation.h"
#include <iostream>
#include <vector>
#include <sstream>
#include "security/random_generation.h"
#include "security/hash.h"
using namespace std;

bool Input_Validation::check_last_digit(const string& d)
{
    if((d[d.size() - 1] - '0') == Random_Generation::last_digit(d))
    {
        return true;
    }
    return false;
}

bool Input_Validation::check_account_no(const string& dest_account)
{
    string att = "920";

    if(dest_account.size() == 12)
    {
        if(dest_account == "111111111111")
        {
            return true;
        }
        else
        {
            for(size_t i = 0; i < att.size(); i ++)
            {
                if(dest_account[i] != att[i])
                {
                    return false;
                }
            }
            if(check_last_digit(dest_account) == true)
            {
                
                return true;
            }
            
            return false;
        }
        
    }
    return false;

}

bool Input_Validation::check_password_length(const string& p)
{
    if(p.size() < 8 || p.size() > 64)
    {
        return false;
    }
    return true;
}

bool Input_Validation::check_new_password(const string& new_p, const string& confirm_new_p)
{
    if(new_p != confirm_new_p)
    {
        return false;
    }
    return true;
}


bool Input_Validation::check_full_name(const string& full_name)
{
    if(full_name.size() >= 2)
    {
        for(size_t i = 0; i < full_name.size(); i ++)
        {
            if(i == 0 || (i == full_name.size() - 1))
            {
                if(!isalpha(full_name[i]))
                {
                    return false;

                }
            }
            else
            {
                if(full_name[i] != ' ' && !isalpha(full_name[i]))
                {
                    return false;
                }
            }

        }
        return true;
    }
    return false;

}

bool Input_Validation::check_salt(vector<unsigned char>& s)
{
    if(s.size() != 16)
    {
        return false;
    }
    return true;
}

bool Input_Validation::check_credential(vector<unsigned char>& c)
{
    if(c.size() != 32)
    {
        return false;
    }
    return true;
}

bool Input_Validation::check_phone_number(const string& pn)
{
    if(pn.size() == 10 && pn[0] == '0')
    {
        for(char c : pn)
        {
            if(!isdigit(c))
            {
                return false;
            }
        }
        return true;
    }
    return false;

}

bool Input_Validation::check_ip_address(const string& ip)
{
    if(!ip.empty())
    {
        stringstream ss;
        string octet;
        while(getline(ss, octet, '.'))
        {
            if(octet.empty())
            {
                return false;
            }
            
            for(char c :octet)
            {
                if(!isdigit(c))
                {
                    return false;
                }
            }

            int num = stoi(octet);

            if(num < 2 || num > 254)
            {
                return false;
            }

        }
        return true;
        
    }
    return false;
}

bool Input_Validation::check_role_id(int r)
{
    if(r < 1 || r > 2)
    {
        return false;
    }
    return true;
}

bool Input_Validation::check_status(const string& s)
{
    if(!s.empty())
    {
        if(s == "ACTIVE" || s == "CLOSED" || s == "LOCKED" || s == "SUSPENDED")
        {
            return true;
        }
        return false;
    }
    return false;
}

bool Input_Validation::check_id(int u)
{
    if(u < 1)
    {
        return false;
    }
    return true;
}

bool Input_Validation::check_balance(double b)
{
    if(b<0)
    {
       return false;
    }
    return true;
}

bool Input_Validation::check_type_account(const string& t)
{
    if(!t.empty())
    {
        if(t == "CURRENT" || t == "BUSINESS" || t == "ADMIN")
        {
            return true;
        }
        return false;

    }
    return false;
}


bool Input_Validation::check_user_input(const string& u_i)
{
    if(!u_i.empty())
    {
        for(char c : u_i)
        {
            if(!isdigit(c))
            {
                return false;

            }
        }
        return true;
    }
    return false;
    
}