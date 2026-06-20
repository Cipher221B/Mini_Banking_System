#pragma once
#include <iostream>
#include <vector>

class Account
{
    private:
        int account_id;
        int user_id;
        std::string account_no;
        double balance; //dynamic
        std::string type;
        std::string status; //dynamic

    public:
        //setter
        void set_account_id(int i);
        void set_user_id(int i);
        void set_account_no(const std::string& n);
        void set_balance(double b);
        void set_type(const std::string& t);
        void set_status(const std::string& s);

        void make_type(std::string t);

        //clear data
        void clear_sensitive_data();
        void clear_all_account_data();

        //getter
        int get_account_id(){return account_id;}
        int get_user_id(){return user_id;}
        std::string get_account_no(){return account_no;}
        double get_balance(){return balance;}
        std::string get_type(){return type;}
        std::string get_status(){return status;}



};

struct Status_Account_Info
{
    public:
        std::string account_no;
        int accountid;
        int userid;
        std::string current_status;
        std::string new_status;

        Status_Account_Info(std::string& acc_no) : account_no(acc_no), userid(0), current_status(""), new_status(""){};
};