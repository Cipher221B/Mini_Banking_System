#pragma once
#include <windows.h>
#include <sqlext.h>
#include "entity/user.h"
#include "entity/account.h"
#include "Shared/database/db_connection.h"
#include "data/transaction_information.h"
#include "error/layer_error.h"

class Account_Repository
{
    private: 
        SQLHDBC hdbc;

    public:
        //constructor
        Account_Repository(SQLHDBC conn);

        //get data api
        double get_balance(Account& a);
        std::string get_date_time(Transaction_Info& t);
        std::string get_status_account(Account& a);
        int get_sequence_account_no();
        Repository_Error::BUSSINESS_ERROR get_dest_account_information(Transaction_Info& t);
        void get_account_information(Account& a);
        Repository_Error::BUSSINESS_ERROR get_status_account_info(Status_Account_Info& sai); 
        
        //change status account api
        void change_status_account(Status_User_Info& sui); 
        void change_status_account(Status_Account_Info& sai);

        //transaction api
        void transaction(Account& a, Transaction_Info& t);

        //register api
        Repository_Error::BUSSINESS_ERROR add_account(Account& a); 

};