#include "user.h"
#include "security/hash.h"
#include "Shared/error/system_errors.h"
#include "utils/input_validation.h"
using namespace std;

//setter
void User::set_phone_number(const string& pn)
{
    if(!Input_Validation::check_phone_number(pn))
    {
        throw Invalid_Input("Phone Number is Invalid.", __FILE__, __LINE__);
    }
    phone_number = pn;

}

void User::set_credential(const string& password)
{
    if(!Input_Validation::check_password_length(password))
    {
        throw Invalid_Input("Password is Invalid (Password length >= 8).", __FILE__, __LINE__);
    }
    creat_salt();
    credential = Hash::encryption(password, salt); 
}

void User::set_credential(vector<unsigned char>& password_hash)
{
    if(!Input_Validation::check_credential(password_hash))
    {
        throw Invalid_Input("Credential is Invalid.", __FILE__, __LINE__); //ghi system log
    }
    credential = password_hash;
}

void User::set_salt(vector<unsigned char>& s)
{
    if(!Input_Validation::check_salt(s))
    {
        throw Invalid_Input("Salt is Invalid.", __FILE__, __LINE__); //ghi system log
    }
    salt = s;

}

void User::set_full_name(const string& f)
{
    if(!Input_Validation::check_full_name(f))
    {
        throw Invalid_Input("Full Name is Invalid.", __FILE__, __LINE__);
    }
    full_name = f;
    
}

void User::set_ip_address(const string& ip)
{
    if(!Input_Validation::check_ip_address(ip))
    {
        throw Invalid_Input("IP Address is Invalid.", __FILE__, __LINE__);
    }
    ip_address = ip;
}

void User::set_role_id(int r)
{
    if(!Input_Validation::check_role_id(r))
    {
        throw Invalid_Input("Role ID is Invalid.", __FILE__, __LINE__);
    }
    role_id = r;
}

void User::set_status_user(const string& s)
{
    if(!Input_Validation::check_status(s))
    {
        throw Invalid_Input("Status User is Invalid.", __FILE__, __LINE__);
    }
    status_user = s;
}

void User::set_user_id(int i)
{
    if(!Input_Validation::check_id(i))
    {
        throw Invalid_Input("User ID is Invalid.", __FILE__, __LINE__);
    }
    user_id = i;
}

void User::creat_salt()
{
    salt = Hash::generate_salt();
}

void User::clear_sensitive_data()
{
    fill(credential.begin(), credential.end(), 0);
    fill(salt.begin(), salt.end(), 0);
    status_user.clear();
    role_id = 0;
}

void User::clear_all_user_data()
{
    user_id = 0;
    full_name.clear();
    phone_number.clear();
    ip_address.clear();
    clear_sensitive_data();
}
