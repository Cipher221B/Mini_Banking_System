#include "log_service.h"
using namespace std;

void LogService::system_log_exception(SQLHDBC hdbc, const string& type_err, const string& namefile_err, int linefile_err, const string& state, int native, const string& message)
{        
    DB_Helper::rollback(hdbc);
    System_Log::error_log(type_err, "system.log", namefile_err, linefile_err, state, native, message);
}

void LogService::system_log_exception(SQLHDBC hdbc, const string& type_err, const string& namefile_err, int linefile_err,const string& message)
{        
    DB_Helper::rollback(hdbc);
    System_Log::error_log(type_err, "system.log",  namefile_err, linefile_err, message);
}

void LogService::safe_audit_login_log(SQLHDBC hdbc, User& u, const string& message, const string& status)
{
    try
    {
        Audit_Log::login(hdbc, u, message, status);
    }
    catch(const Create_Log_Error& e)
    {
        System_Log::error_log("Create Log Failed", "system.log",  e.filename_err, e.line_err, e.state, e.native_err, e.what());
    }

}

void LogService::safe_audit_login_log(SQLHDBC hdbc, User& u, Account& a, const string& message, const string& status)
{
    try
    {
        Audit_Log::login(hdbc, u, a, message, status);
    }
    catch(const Create_Log_Error& e)
    {
        System_Log::error_log("Create Log Failed", "system.log",  e.filename_err, e.line_err, e.state, e.native_err, e.what());
    }

}

void LogService::safe_audit_user_not_available_log(SQLHDBC hdbc, User& u)
{
    try
    {
        Audit_Log::user_not_available(hdbc, u);
        
    }
    catch(const Create_Log_Error& e)
    {
        System_Log::error_log("Create Log Failed", "system.log",  e.filename_err, e.line_err, e.state, e.native_err, e.what());
    }

}

void LogService::safe_audit_user_not_found_log(SQLHDBC hdbc, User& u)
{
    try
    {
        Audit_Log::user_not_found(hdbc, u);
        
    }
    catch(const Create_Log_Error& e)
    {
        System_Log::error_log("Create Log Failed", "system.log",  e.filename_err, e.line_err, e.state, e.native_err, e.what());
    }
}
