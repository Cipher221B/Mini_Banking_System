#pragma once

#include <iostream>
#include <vector>
using namespace std;

class Account
{
    private:
        int account_id;
        int user_id;
        string account_no;
        double balance; //dynamic
        string type;
        string status; //dynamic

    public:
        //setter
        void set_account_id(int i);
        void set_user_id(int i);
        void set_account_no(const string& n);
        void set_balance(double b);
        void set_type(const string& t);
        void set_status(const string& s);

        void make_type(int c);

        //getter
        int get_account_id(){return account_id;}
        int get_user_id(){return user_id;}
        string get_account_no(){return account_no;}
        double get_balance(){return balance;}
        string get_type(){return type;}
        string get_status(){return status;}



};