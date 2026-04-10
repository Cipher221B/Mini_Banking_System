#include "validation_helper.h"
#include <iostream>
#include <vector>
#include "random_generation.h"
#include "hash.h"

bool Validate::check_last_digit(string& d)
{
    if ((d[d.size() - 1] - '0') == last_digit(d))
    {
        return true;
    }
    return false;
}

bool Validate::check_account_no(string& dest_account)
{
    string att = "920";

    if(dest_account.size() == 12)
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

    return false;

}

bool Validate::check_status_user(User& u)
{
    if(u.status_user == "ACTIVE")
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Validate::verify(User& u, string& p)
{
    vector<unsigned char> cre = Hash::encryption(p, u.salt);

    if(cre == u.credential)
    {
        return true;
    }
    else
    {
        return false;
    }

}

string Validate::check_role(User& u, Account& a)
{
    switch(u.role_id)
    {
        case 1:
        {
            return "ADMIN";
            
        }
        case 2:
        {
            return "CUSTOM";
        }

    }
    return "CUSTOM";

}

bool Validate::check_ssession(Session& s, int current_session_on_db)
{
    if(s.get_session_version() != current_session_on_db)
    {
        return false;
    }
    return true;

}