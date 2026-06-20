#pragma once
#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include "Shared/utils/db_helper.h"
#include "Shared/logging/system_log.h" 
#include "logging/audit_log.h"
#include "Shared/error/database_errors.h"

class LogService
{
    public:
        //register
        static void system_log_exception(SQLHDBC hdbc, const std::string& type_err, const std::string& namefile_err, int linefile_err, const std::string& state, int native, const std::string& message);
        static void system_log_exception(SQLHDBC hdbc, const std::string& type_err, const std::string& namefile_err, int linefile_err, const std::string& message);
        
        //login
        static void safe_audit_login_log(SQLHDBC hdbc, User& u, const std::string& message, const std::string& status);
        static void safe_audit_login_log(SQLHDBC hdbc, User& u, Account& a, const std::string& message, const std::string& status);
        static void safe_audit_user_not_available_log(SQLHDBC hdbc, User& u);
        static void safe_audit_user_not_found_log(SQLHDBC hdbc, User& u);

};