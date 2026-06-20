#pragma once
#include "entity/account.h"
#include "repository/account_repository.h"

//3 bank code (default) [920] + 8 random number + 1 check digit (sum(total(11 digit))%9)
namespace Random_Generation
{
    int generate_number(int start, int end);
    std::string get_unix_timestamp();
    std::string create_sequence_ms(Account_Repository& ar);
    std::string create_check_digit(const std::string& sn);
    std::string generate_account_no(Account_Repository& ar);

    //check last digit
    int last_digit(const std::string& sn);

    //create transaction code
    std::string generate_transaction_code();

    //create ip
    std::string generate_ip();
}

