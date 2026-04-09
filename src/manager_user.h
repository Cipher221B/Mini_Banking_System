#pragma once

#include <iostream>
#include <vector>
using namespace std;

class User
{
    
    private:
        int user_id;
        string full_name;
        string phone_number;
        vector<unsigned char> credential; //password
        vector<unsigned char> salt;
        string status_user;
        int role_id;

    public: 
        //Constructor
        User() : credential(32), salt(16){};
        
        //setter api
        void creat_salt();
        void set_salt(vector<unsigned char>& password_hash);
        void set_credential(const string& password);
        void set_credential(vector<unsigned char>& password_hash);
        void set_phone_number(const string& pn);
        void set_user_id(int i);
        void set_status_user(const string& s);
        void set_role_id(int r);
        void set_full_name(const string& f);

        //getter api
        int get_user_id() {return user_id;}
        string get_full_name() {return full_name;}
        string get_phone_number() {return phone_number;}
        vector<unsigned char> get_credential() {return credential;}
        vector<unsigned char> get_salt() {return salt;}
        string get_status_user(){return status_user;}
        int get_role_id(){return role_id;}

};