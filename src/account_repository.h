#pragma once

#include <windows.h>
#include <sqlext.h>
#include "manager_user.h"
#include "manager_account.h"
#include "db_connection.h"
#include "sql_error.h"
#include "transaction_information.h"

class Account_Repository
{
    private: 
        SQLHDBC hdbc;
        Error_Info last_err;

    public:
        enum class Repository_Result
        {
            NONE,
            SUCCESS,
            FAILED,
            DUPLICATE_ACCOUNT_NO, 
            ACCOUNT_NOT_FOUND, 
            SENDER_NOT_EXIST,
            RECEIVER_NOT_EXIST,
            SENDER_NOT_AVAILABLE,
            RECEIVER_NOT_AVAILABLE,
            INSUFFICIENT_BALANCE
            //TRANSACTION_FAILED
        };

        Account_Repository(SQLHDBC conn);

        //handle error register api
        void handle_error_register(SQLSMALLINT type, SQLHANDLE handle);
        void handle_error_register(SQLSMALLINT type_err, SQLHANDLE handle_err, SQLSMALLINT type_free, SQLHANDLE handle_free);
        //void handle_dup_create_account_no_system();

        //handle error login api
        void handle_error_login(SQLSMALLINT type, SQLHANDLE handle);
        void handle_error_login(SQLSMALLINT type_err, SQLHANDLE handle_err, SQLSMALLINT type_free, SQLHANDLE handle_free);

        //handle error transaction api
        void handle_error_transaction(SQLSMALLINT type, SQLHANDLE handle);
        void handle_error_transaction(SQLSMALLINT type_err, SQLHANDLE handle_err, SQLSMALLINT type_free, SQLHANDLE handle_free);

        //transaction api
        Repository_Result transaction(Account& a, string& dest_account, int amount);
        int get_sequence_account_no(Account& a);
        int get_sequence_transaction_code(Account& a);
        // void get_dest_account_information(Transaction_Info& tran_in);


        //login api
        Repository_Result get_information_account(Account& a); //DUPLICATE_ACCOUNT_NO

        //register api
        Repository_Result add_account(User& u, Account& a); //ACCOUNT_NOT_FOUND


};