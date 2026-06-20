#pragma once
#include <iostream>
#include <vector>

struct Transaction_Info
{
    public:
        //transaction info
        std::string transaction_code;
        //Account Info
        int dest_account_id;
        std::string dest_account_no; //
        std::string full_name;
        double amount; //
        std::string datetime;


        Transaction_Info(std::string& d, double& a) : transaction_code(""), dest_account_id(0), dest_account_no(d), full_name(""), amount(a), datetime(""){};
};