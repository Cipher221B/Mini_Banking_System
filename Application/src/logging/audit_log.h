#pragma once
#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include "entity/user.h"
#include "entity/account.h"
#include "Shared/database/db_connection.h"
#include "data/transaction_information.h"
#include "error/layer_error.h"

class Audit_Log
{
    public:
        // change credential
        static void change_password(SQLHDBC hdbc, User& u, const std::string& status_activity);

        // change status
        static void change_status_user(SQLHDBC hdbc, User& u, Status_User_Info& sui, const std::string& status_activity);
        static void change_status_account(SQLHDBC hdbc, User& u, Status_Account_Info& sai, const std::string& status_activity);

        //transaction log api
        static Repository_Error::BUSSINESS_ERROR create_transaction_audit(SQLHDBC hdbc, Account& a, Transaction_Info& t, const std::string& status);

        //login log api
        static void user_not_found(SQLHDBC hdbc, User& u);
        static void user_not_available(SQLHDBC hdbc, User& u);
        static void login(SQLHDBC hdbc, User& u, const std::string& message, const std::string& status);
        static void login(SQLHDBC hdbc, User& u, Account& a, const std::string& message, const std::string& status);

        //register log api
        static void account_register(SQLHDBC hdbc, User& u, Account& a, const std::string& message, const std::string& status);
        static void user_register(SQLHDBC hdbc, User& u, const std::string& message, const std::string& status);
       
        //logout log api
        static void logout(SQLHDBC hdbc, User& u, Account& a, const std::string& message, const std::string& status);


};