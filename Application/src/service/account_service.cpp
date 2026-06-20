#include "account_service.h"
#include "repository/user_repository.h"
#include "repository/account_repository.h"
#include "Shared/error/database_errors.h"
#include "error/bussiness_error.h"
#include "Shared/logging/system_log.h"
#include "utils/log_service.h"
#include "service/auth_service.h"
#include "security/random_generation.h"
#include "security/hash.h"
using namespace std;

//AccountService
LOAD_DATA AccountService::refresh(Session& s, Account& a)
{
    try
    {
        DataBase conn;
        AuthService aus;
        DB_Helper::begin_transaction(conn.get_hdbc());

        try
        {
            //Get things that don't affect the system but are important.
            User_Repository ur(conn.get_hdbc());
            if(!aus.check_session_version(s, ur.get_version_db(s)))
            {
                return {Service_Error::SYSTEM_ERROR::NONE, Service_Error::BUSSINESS_ERROR::NONE , Service_Error::VALIDATE_ERROR::SESSION_VERSION_EXPIRED};
            }
            Account_Repository ar(conn.get_hdbc());
            a.set_balance(ar.get_balance(a));
            DB_Helper::commit(conn.get_hdbc());
        }
        catch(const Get_Data_Error& e)
        {
            LogService::system_log_exception(conn.get_hdbc(), "Get Data From DataBase Failed", e.filename_err, e.line_err, e.state, e.native_err, e.what());
            return {Service_Error::SYSTEM_ERROR::FETCH_FAILED};
        }
        catch(const Invalid_Input& e)
        {
            System_Log::error_log("Invalid Data Detected", "system.log", e.filename_err, e.line_err, e.what());
            return {Service_Error::SYSTEM_ERROR::DATABASE_ERROR};
        }
    }
    catch(const ConnectionErrors& e)
    {
        System_Log::error_log("DataBase Connection Failed", "system.log", e.filename_err, e.line_err, e.state, e.native_err, e.what());
        return {Service_Error::SYSTEM_ERROR::CONNECTION_FAILED};
    }
    return {};

}


LOAD_DATA AccountService::get_account_status_info(Status_Account_Info& sai)
{
    try
    {
        DataBase conn; //Data Base Connection Error
        Account_Repository ar(conn.get_hdbc());
        Repository_Error::BUSSINESS_ERROR repr = ar.get_status_account_info(sai);
        if(repr == Repository_Error::BUSSINESS_ERROR::ACCOUNT_NOT_FOUND)
        {
            return {Service_Error::SYSTEM_ERROR::NONE, Service_Error::BUSSINESS_ERROR::ACCOUNT_NOT_FOUND, Service_Error::VALIDATE_ERROR::NONE};
        }
       
    }
    catch(const ConnectionErrors& e)
    {
        System_Log::error_log("DataBase Connection Failed", "system.log", e.filename_err, e.line_err, e.state, e.native_err, e.what());
        return {Service_Error::SYSTEM_ERROR::CONNECTION_FAILED};
    }
    catch(const Get_Data_Error& e)
    {
        System_Log::error_log("Get Data From DataBase Failed", "system.log", e.filename_err, e.line_err, e.state, e.native_err, e.what());
        return {Service_Error::SYSTEM_ERROR::FETCH_FAILED};
    }
    return {};
}


LOAD_DATA AccountService::get_user_status_info(Status_User_Info& sui)
{
    try
    {
        DataBase conn; //Data Base Connection Error
        User_Repository ur(conn.get_hdbc());
        Repository_Error::BUSSINESS_ERROR repr = ur.get_status_user_info(sui);
        if(repr == Repository_Error::BUSSINESS_ERROR::PHONE_NUMBER_DOES_NOT_EXIST)
        {
            return {Service_Error::SYSTEM_ERROR::NONE, Service_Error::BUSSINESS_ERROR::USER_NOT_EXIST, Service_Error::VALIDATE_ERROR::NONE};
        }
       
    }
    catch(const ConnectionErrors& e)
    {
        System_Log::error_log("DataBase Connection Failed", "system.log",  e.filename_err, e.line_err, e.state, e.native_err, e.what());
        return {Service_Error::SYSTEM_ERROR::CONNECTION_FAILED};
    }
    catch(const Get_Data_Error& e)
    {
        System_Log::error_log("Get Data From DataBase Failed", "system.log",  e.filename_err, e.line_err, e.state, e.native_err, e.what());
        return {Service_Error::SYSTEM_ERROR::FETCH_FAILED};
    }
    return {};
}

//CustomService
TRANSFER UserService::create_transaction(Session& s, User& u, Account& a, Transaction_Info& t, string& p)
{
    try
    {
        DataBase conn; //Data Base Connection Error
        User_Repository ur(conn.get_hdbc());
        Account_Repository ar(conn.get_hdbc());
        AuthService aus;
        Service_Error::VALIDATE_ERROR v = aus.action_verification_on_account(s, ar, ur, a, u, p);
        if(v == Service_Error::VALIDATE_ERROR::SESSION_VERSION_EXPIRED || v == Service_Error::VALIDATE_ERROR::USER_NOT_AVAILABLE || v == Service_Error::VALIDATE_ERROR::ACCOUNT_NOT_AVAILABLE || v == Service_Error::VALIDATE_ERROR::VERIFY_FAILED)
        {
            return {Service_Error::SYSTEM_ERROR::NONE, Service_Error::BUSSINESS_ERROR::NONE, v};
        }
        Repository_Error::BUSSINESS_ERROR repr = ar.get_dest_account_information(t);
        if(repr == Repository_Error::BUSSINESS_ERROR::ACCOUNT_NOT_FOUND)
        {
            return {Service_Error::SYSTEM_ERROR::NONE, Service_Error::BUSSINESS_ERROR::RECEIVER_NOT_EXIST, Service_Error::VALIDATE_ERROR::NONE};

        }
        else if(a.get_account_id() == t.dest_account_id)
        {
            return {Service_Error::SYSTEM_ERROR::NONE, Service_Error::BUSSINESS_ERROR::SAME_ACCOUNT_TRANSFER, Service_Error::VALIDATE_ERROR::NONE};
        }
        DB_Helper::begin_transaction(conn.get_hdbc());
        try
        {
            ar.transaction(a, t);
            constexpr int MAX_RETRY = 5;
            for(int attempt = 0; attempt < MAX_RETRY; attempt++)
            {
                t.transaction_code = Random_Generation::generate_transaction_code();
                Repository_Error::BUSSINESS_ERROR repr = Audit_Log::create_transaction_audit(conn.get_hdbc(), a, t, "SUCCESS");
                if(repr == Repository_Error::BUSSINESS_ERROR::NONE)
                {
                    break;
                }
                if(attempt == (MAX_RETRY - 1) && repr == Repository_Error::BUSSINESS_ERROR::DUPLICATE_TRANSACTION_NO)
                {
                    return {Service_Error::SYSTEM_ERROR::NONE, Service_Error::BUSSINESS_ERROR::E_DUPLICATE_TRANSACTION_NO};                  
                }
            }
            t.datetime = ar.get_date_time(t);
            DB_Helper::commit(conn.get_hdbc());
        }
        catch(const Transaction_Error& e)
        {
            switch(e.native_err)
            {
                case(50001):
                {
                    return {Service_Error::SYSTEM_ERROR::NONE, Service_Error::BUSSINESS_ERROR::NONE, Service_Error::VALIDATE_ERROR::RECEIVER_NOT_AVAILABLE};
                    
                }
                case(50002):
                {
                    return {Service_Error::SYSTEM_ERROR::NONE, Service_Error::BUSSINESS_ERROR::INSUFFICIENT_BALANCE, Service_Error::VALIDATE_ERROR::NONE};
                }
                default:
                {
                    LogService::system_log_exception(conn.get_hdbc(), "Transaction Failed", e.filename_err, e.line_err, e.state, e.native_err, e.what());
                    return {Service_Error::SYSTEM_ERROR::DATABASE_ERROR};
                }
            }
        }
        catch(const Open_File_Error& e)
        {
            System_Log::error_log("Get Data From File Failed Detected", "system.log",  e.filename_err, e.line_err, e.what());
            return {Service_Error::SYSTEM_ERROR::DATABASE_ERROR};
        }
        catch(const Create_Log_Error& e)
        {
            LogService::system_log_exception(conn.get_hdbc(), "Transaction Failed", e.filename_err, e.line_err, e.state, e.native_err, e.what());
            return {Service_Error::SYSTEM_ERROR::DATABASE_ERROR};
        }
    }
    catch(const ConnectionErrors& e)
    {
        System_Log::error_log("DataBase Connection Failed", "system.log",  e.filename_err, e.line_err, e.state, e.native_err, e.what());
        return {Service_Error::SYSTEM_ERROR::CONNECTION_FAILED};
    }
    catch(const Get_Data_Error& e)
    {
        System_Log::error_log("Get Data From DataBase Failed", "system.log",  e.filename_err, e.line_err, e.state, e.native_err, e.what());
        return {Service_Error::SYSTEM_ERROR::FETCH_FAILED};
    }
    catch(const Hash_Algorithm_Error& e)
    {
        System_Log::error_log("Login Hash Failed", "system.log",  e.filename_err, e.line_err, e.what());
        return {Service_Error::SYSTEM_ERROR::HASH_FAILED};
    }
    catch(const Invalid_Input& e)
    {
        System_Log::error_log("Invalid Data Detected", "system.log",  e.filename_err, e.line_err, e.what());
        return {Service_Error::SYSTEM_ERROR::DATABASE_ERROR};
    }
    return {};

}

//AdminService
CHANGE_STATUS AdminService::change_status_account(Session& s, User& u, Account& a, Status_Account_Info& sai, string& p)
{
    try
    {
        DataBase conn; //Data Base Connection Error
        User_Repository ur(conn.get_hdbc());
        Account_Repository ar(conn.get_hdbc());
        AuthService aus;
        Service_Error::VALIDATE_ERROR v = aus.action_verification_on_account(s, ar, ur, a, u, p);
        if(v == Service_Error::VALIDATE_ERROR::SESSION_VERSION_EXPIRED || v == Service_Error::VALIDATE_ERROR::USER_NOT_AVAILABLE || v == Service_Error::VALIDATE_ERROR::ACCOUNT_NOT_AVAILABLE || v == Service_Error::VALIDATE_ERROR::VERIFY_FAILED)
        {
            return {Service_Error::SYSTEM_ERROR::NONE, v};
        }

        DB_Helper::begin_transaction(conn.get_hdbc());
        try
        {
            ar.change_status_account(sai);
            Audit_Log::change_status_account(conn.get_hdbc(), u, sai, "SUCCESS");
            DB_Helper::commit(conn.get_hdbc());
        }
        catch(const Change_Status_Account_Error& e)
        {
            LogService::system_log_exception(conn.get_hdbc(), "Change Status Account Failed", e.filename_err, e.line_err, e.state, e.native_err, e.what());
            return {Service_Error::SYSTEM_ERROR::CHANGE_STATUS_ACCOUNT_FAILED};
        }
        catch(const Create_Log_Error& e)
        {
            LogService::system_log_exception(conn.get_hdbc(), "Audit Log Failed Detected", e.filename_err, e.line_err, e.state, e.native_err, e.what());
            return {Service_Error::SYSTEM_ERROR::DATABASE_ERROR};
        }
        
    }
    catch(const ConnectionErrors& e)
    {
        System_Log::error_log("DataBase Connection Failed", "system.log",  e.filename_err, e.line_err, e.state, e.native_err, e.what());
        return {Service_Error::SYSTEM_ERROR::CONNECTION_FAILED};
    }
    catch(const Get_Data_Error& e)
    {
        System_Log::error_log("Get Data From DataBase Failed", "system.log",  e.filename_err, e.line_err, e.state, e.native_err, e.what());
        return {Service_Error::SYSTEM_ERROR::FETCH_FAILED};
    }
    catch(const Hash_Algorithm_Error& e)
    {
        System_Log::error_log("Login Hash Failed", "system.log",  e.filename_err, e.line_err, e.what());
        return {Service_Error::SYSTEM_ERROR::HASH_FAILED};
    }
    catch(const Invalid_Input& e)
    {
        System_Log::error_log("Invalid Data Detected", "system.log",  e.filename_err, e.line_err, e.what());
        return {Service_Error::SYSTEM_ERROR::DATABASE_ERROR};
    }
    return {};
}

CHANGE_STATUS AdminService::change_status_user(Session& s, User& u, Account& a, Status_User_Info& sui, string& p)
{
    try
    {
        DataBase conn; //Data Base Connection Error
        User_Repository ur(conn.get_hdbc());
        Account_Repository ar(conn.get_hdbc());
        AuthService aus;
        Service_Error::VALIDATE_ERROR v = aus.action_verification_on_account(s, ar, ur, a, u, p);
        if(v == Service_Error::VALIDATE_ERROR::SESSION_VERSION_EXPIRED || v == Service_Error::VALIDATE_ERROR::USER_NOT_AVAILABLE || v == Service_Error::VALIDATE_ERROR::ACCOUNT_NOT_AVAILABLE || v == Service_Error::VALIDATE_ERROR::VERIFY_FAILED)
        {
            return {Service_Error::SYSTEM_ERROR::NONE, v};
        }

        DB_Helper::begin_transaction(conn.get_hdbc());
        try
        {
            ur.change_status_user(sui);
            ar.change_status_account(sui);
            Audit_Log::change_status_user(conn.get_hdbc(), u, sui, "SUCCESS");
            DB_Helper::commit(conn.get_hdbc());
        }
        catch(const Change_Status_User_Error& e)
        {
            LogService::system_log_exception(conn.get_hdbc(), "Change Status User Failed", e.filename_err, e.line_err, e.state, e.native_err, e.what());
            return {Service_Error::SYSTEM_ERROR::CHANGE_STATUS_USER_FAILED};
        }
        catch(const Create_Log_Error& e)
        {
            LogService::system_log_exception(conn.get_hdbc(), "Audit Log Failed Detected", e.filename_err, e.line_err, e.state, e.native_err, e.what());
            return {Service_Error::SYSTEM_ERROR::DATABASE_ERROR};
        }
        
    }
    catch(const ConnectionErrors& e)
    {
        System_Log::error_log("DataBase Connection Failed", "system.log",  e.filename_err, e.line_err, e.state, e.native_err, e.what());
        return {Service_Error::SYSTEM_ERROR::CONNECTION_FAILED};
    }
    catch(const Get_Data_Error& e)
    {
        System_Log::error_log("Get Data From DataBase Failed", "system.log",  e.filename_err, e.line_err, e.state, e.native_err, e.what());
        return {Service_Error::SYSTEM_ERROR::FETCH_FAILED};
    }
    catch(const Hash_Algorithm_Error& e)
    {
        System_Log::error_log("Login Hash Failed", "system.log",  e.filename_err, e.line_err, e.what());
        return {Service_Error::SYSTEM_ERROR::HASH_FAILED};
    }
    catch(const Invalid_Input& e)
    {
        System_Log::error_log("Invalid Data Detected", "system.log",  e.filename_err, e.line_err, e.what());
        return {Service_Error::SYSTEM_ERROR::DATABASE_ERROR};
    }
    return {};
}
