#pragma once

#include <iostream>
#include "manager_account.h"
#include "manager_user.h"
using namespace std;
class Validate
{
    public:
        static bool check_account_no(string& dest_account);
        static bool check_last_digit(string& d);
        static bool check_balance(Account& a, int amount);
        static bool check_permission_to_create_transaction(User& u);
        static bool check_status_user(User& u);
        static bool verify(User& u, string& p);
        static string check_role(User& u, Account& a);
};