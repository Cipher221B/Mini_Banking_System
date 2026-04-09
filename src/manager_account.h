#pragma once

#include <iostream>
#include <vector>
using namespace std;

struct Account
{
    public:
        int account_id;
        int user_id;
        string account_no;
        double balance; //dynamic
        string type;
        string status; //dynamic

        void create_account();
        bool set_type_account(int choose);

};