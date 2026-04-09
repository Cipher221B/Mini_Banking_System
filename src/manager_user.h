#pragma once

#include <iostream>
#include <vector>
using namespace std;

struct User
{
    int user_id;
    string full_name;
    string phone_number;
    vector<unsigned char> credential; //password
    vector<unsigned char> salt;
    string status_user;
    int role_id;

    //Constructor
    User() : credential(32), salt(16){};
    
    string input_phone();
    string input_password();
    void create_user();
    void add_phone_number();
    void add_password();
    void set_salt();
    void set_credential(string& password);
    void set_phone_number(string& pn);
    bool check_valid_phone(string& pn);
    bool check_valid_password(string& p);

};