#include "manager_user.h"
#include "hash.h"

string User::input_phone()
{
    string pn;
    cout << "Phone Number('Include 10 digit and start with 0'): ";
    getline(cin, pn);
    return pn;
}

string User::input_password()
{
    string p;
    cout << "Password('At Least 8 character'): ";
    getline(cin, p);
    return p;

}

bool User::check_valid_phone(string& pn)
{
    if(pn.size() == 10 && pn[0] == '0')
    {
        return true;
    }
    else
    {
        cout << "Error Phone Number!!! Try Again!\n";
        return false;
    }

}

bool User::check_valid_password(string& p)
{
    if(p.size() >= 8 && p.size() <= 64)
    {
        return true;
    }
    else
    {
        cout << "Error Password!!! Try Again!\n";
        return false;
    }
}

void User::set_phone_number(string& pn)
{
    phone_number = pn;

}

void User::set_credential(string& password)
{
    credential = Hash::encryption(password, salt); // throw runtime error
}

void User::set_salt()
{
    salt = Hash::generate_salt();
}

void User::add_phone_number()
{
    string raw_phone;
    do
    {
        raw_phone = input_phone();

    } while(!check_valid_phone(raw_phone));
    set_phone_number(raw_phone);

}

void User::add_password()
{
    string raw_password;
    do
    {
        raw_password = input_password();

    }while(!check_valid_password(raw_password));
    set_salt();
    set_credential(raw_password);
}

void User::create_user()
{
 
    cout << "Full Name: ";
    getline(cin, full_name);

    add_phone_number();
    add_password();
    status_user = "ACTIVE";
    role_id = 2;
    
}

