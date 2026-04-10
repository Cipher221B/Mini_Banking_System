#include "manager_user.h"
#include "hash.h"
#include "system_errors.h"

//setter
void User::set_phone_number(const string& pn)
{
    if(pn.size() == 10 && pn[0] == '0')
    {
        phone_number = pn;
    }
    throw Invalid_Input("Phone Number is Invalid.", __FILE__);

}

void User::set_credential(const string& password)
{
    if(password.size() >= 8 && password.size() <= 64)
    {
        creat_salt();
        credential = Hash::encryption(password, salt); 
    }
    throw Invalid_Input("Password is Invalid (Password length >= 8).", __FILE__);
}

void User::set_credential(vector<unsigned char>& password_hash)
{
    if(password_hash == 32)
    {
        credential = password_hash;
    }
    throw Invalid_Input("Credential is Invalid.", __FILE__); //ghi system log
}

void User::set_salt(vector<unsigned char>& s)
{
    if(s.size() == 16)
    {
        salt = s;
    }
    throw Invalid_Input("Salt is Invalid.", __FILE__); //ghi system log

}

void User::set_full_name(const string& f)
{
    if(f.size() < 2)
    {
        throw Invalid_Input("Full Name is Invalid.", __FILE__);
    }
    full_name = f;
    
}

void User::set_role_id(int r)
{
    role_id = r;
}

void User::set_status_user(const string& s)
{
    status_user = s;
}

void User::set_user_id(int i)
{
    id = i;
}

void User::creat_salt()
{
    salt = Hash::generate_salt();
}

void User::clear_sensitive_data()
{
    fill(credential.begin(), credential.end(), 0);
    credential.clear();

    fill(salt.begin(), salt.end(), 0);
    salt.clear();

    status_user;

    role_id = 0;
}

void User::clear_all_user_data()
{
    user_id = 0;
    full_name.clear();
    phone_number.clear();

    clear_dynamic_data();
}
