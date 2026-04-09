#pragma once
#include <iostream>
#include <vector>
using namespace std;

struct Transaction_Info
{
    public:
        string dest_account_no; //
        double amount;

        //Account Info
        int dest_account_id;

        //User Info
        int dest_user_id;
        string dest_full_name_account; //


        int dest_transaction_id;
        string transaction_code; //

    


        Transaction_Info(string& d, int a);

};