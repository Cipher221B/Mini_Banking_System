#pragma once

#include "manager_account.h"
#include "account_repository.h"

//3 bank code (default) [920] + 8 random number + 1 check digit (sum(total(11 digit))%9)
string generate_number();
string get_unix_timestamp();
string creat_sequence_ms(Account& a, Account_Repository& ar);
string create_check_digit(string& sn);
string create_account_no(Account& a, Account_Repository& ar);

//check last digit
int last_digit(string& sn);


//create transaction code
string create_transaction_code();
