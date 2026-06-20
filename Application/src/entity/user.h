#pragma once
#include <iostream>
#include <vector>

class User
{
    
    private:
        int user_id;
        std::string full_name;
        std::string phone_number;
        std::vector<unsigned char> credential; //password
        std::vector<unsigned char> salt;
        std::string status_user;
        int role_id;
        std::string ip_address;

    public: 
        //Constructor
        User() : credential(32), salt(16){};
        
        //clear data
        void clear_sensitive_data();
        void clear_all_user_data();
        
        //setter api
        void creat_salt();
        void set_salt(std::vector<unsigned char>& password_hash);
        void set_credential(const std::string& password);
        void set_credential(std::vector<unsigned char>& password_hash);
        void set_phone_number(const std::string& pn);
        void set_user_id(int i);
        void set_status_user(const std::string& s);
        void set_role_id(int r);
        void set_full_name(const std::string& f);
        void set_ip_address(const std::string& ip);

        //getter api
        int get_user_id() {return user_id;}
        std::string get_full_name() {return full_name;}
        std::string get_phone_number() {return phone_number;}
        std::vector<unsigned char> get_credential() {return credential;}
        std::vector<unsigned char> get_salt() {return salt;}
        std::string get_status_user(){return status_user;}
        int get_role_id(){return role_id;}
        std::string get_ip_address() {return ip_address;}

};

struct Status_User_Info
{
    public:
        std::string phone_number;
        int userid;
        std::string full_name;
        std::string current_status;
        std::string new_status;

        Status_User_Info(std::string& p) : phone_number(p), userid(0), full_name(""), current_status(""), new_status(""){};
};