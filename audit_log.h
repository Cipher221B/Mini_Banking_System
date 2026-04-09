#pragma once
#include <iostream>
using namespace std;

#include <windows.h>
#include <sqlext.h>
#include "manager_user.h"
#include "manager_account.h"
#include "db_connection.h"

class Audit_Log
{
    public:
        //handle error api
        static void handle_error(SQLSMALLINT type_err, SQLHANDLE handle_err, SQLSMALLINT type_free, SQLHANDLE handle_free);
        static void handle_error(SQLSMALLINT type, SQLHANDLE handle);

        //transaction log api
        static void transaction(SQLHDBC hdbc, Account& a, const string& dest_account_no, const string& status, int amount);

        //register log api
        static void user_register(SQLHDBC hdbc, User& u, const string& message, const string& status);
        static void account_register(SQLHDBC hdbc, Account& a, const string& message, const string& status);
       
        //login log api
        static void user_not_found(SQLHDBC hdbc, User& u);
        static void user_not_available(SQLHDBC hdbc, User& u);
        static void login(SQLHDBC hdbc, User& u, const string& message, const string& status);
        static void login(SQLHDBC hdbc, Account& a, const string& message, const string& status);


};