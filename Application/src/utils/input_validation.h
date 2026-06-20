#pragma once
#include <iostream>
#include "entity/account.h"
#include "entity/user.h"
#include "entity/session.h"

namespace Input_Validation
{
    bool check_password_length(const std::string& p);
    bool check_new_password(const std::string& new_p, const std::string& confirm_new_p);
    bool check_account_no(const std::string& dest_account);
    bool check_last_digit(const std::string& d);
    bool check_full_name(const std::string& full_name);
    bool check_salt(std::vector<unsigned char>& s);
    bool check_credential(std::vector<unsigned char>& c);
    bool check_phone_number(const std::string& pn);
    bool check_ip_address(const std::string& ip);
    bool check_status(const std::string& s);
    bool check_role_id(int r);
    bool check_id(int u);
    bool check_balance(double b);
    bool check_type_account(const std::string& t);
    bool check_user_input(const std::string& u_i);
}