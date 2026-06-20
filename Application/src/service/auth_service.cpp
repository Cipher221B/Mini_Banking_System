#include "auth_service.h"
#include <stdexcept>
#include <vector>
#include "Shared/utils/db_helper.h"
#include "Shared/logging/system_log.h"
#include "security/hash.h"
#include "security/random_generation.h"
#include "Shared/error/database_errors.h"
#include "Shared/error/system_errors.h"
#include "logging/audit_log.h"
#include "account_service.h"
#include "entity/session.h"
#include "error/bussiness_error.h"
#include "utils/log_service.h"
#include "Shared/utils/file_handle.h"
#include "utils/ip_manager.h"
using namespace std;

// private check logic auth
bool AuthService::check_session_version(Session& s, int current_session_on_db)
{
    if(s.get_session_version() != current_session_on_db)
    {
        return false;
    }
    return true;

}

bool AuthService::check_status_user(User& u)
{
    if(u.get_status_user() == "ACTIVE")
    {
        return true;
    }
 
    return false;
}

bool AuthService::check_status_account(Account& a)
{
    if(a.get_status() == "ACTIVE")
    {
        cout << "status active\n";
        return true;
    }
    cout << "status not active\n";

    return false;
}


bool AuthService::verify(User& u, string& p)
{
    vector<unsigned char> cre = Hash::encryption(p, u.get_salt());

    if(cre == u.get_credential())
    {
        return true;
    }
    return false;

}

Service_Error::VALIDATE_ERROR AuthService::action_verification_on_user(Session& s, User_Repository& ur, User& u, string& password)
{
    if(check_session_version(s, ur.get_version_db(s)))
    {
        ur.get_authentication_data(u);
        Service_Error::VALIDATE_ERROR v = login_verification(u, password);
        return v;
    }
    return Service_Error::VALIDATE_ERROR::SESSION_VERSION_EXPIRED;
} 

Service_Error::VALIDATE_ERROR AuthService::action_verification_on_account(Session& s, Account_Repository& ar, User_Repository& ur, Account& a, User& u, string& password)
{
    if(check_session_version(s, ur.get_version_db(s)))
    {
        ur.get_authentication_data(u);
        Service_Error::VALIDATE_ERROR v = login_verification(u, password);
        if(v == Service_Error::VALIDATE_ERROR::NONE)
        {
            a.set_status(ar.get_status_account(a));
            if(!check_status_account(a))
            {
                return Service_Error::VALIDATE_ERROR::ACCOUNT_NOT_AVAILABLE;
            }
            return Service_Error::VALIDATE_ERROR::NONE;

        }
        return v;
    }
    return Service_Error::VALIDATE_ERROR::SESSION_VERSION_EXPIRED;
}

Service_Error::VALIDATE_ERROR AuthService::login_verification(User& u, string& password)
{
    if(verify(u, password))
    {
        if(!check_status_user(u))
        {
            return Service_Error::VALIDATE_ERROR::USER_NOT_AVAILABLE;
        }
        return Service_Error::VALIDATE_ERROR::NONE;
    }
    return Service_Error::VALIDATE_ERROR::VERIFY_FAILED;
}

Service_Error::BUSSINESS_ERROR AuthService::load_authenticated_user(User& u, Session& s, User_Repository& ur, string& phone_number)
{
    Repository_Error::BUSSINESS_ERROR repr = ur.get_user_data_to_authentication(u, s, phone_number);
    if(repr == Repository_Error::BUSSINESS_ERROR::PHONE_NUMBER_DOES_NOT_EXIST)
    {
        return Service_Error::BUSSINESS_ERROR::USER_NOT_EXIST;
    }
    return Service_Error::BUSSINESS_ERROR::NONE;
    
}

CHANGE_PASSWORD AuthService::change_password(Session& s, User& u, string& current_p, string& new_p)
{
    try
    {
      
        DataBase conn; //Data Base Connection Error
        User_Repository ur(conn.get_hdbc());
        Service_Error::VALIDATE_ERROR v = action_verification_on_user(s, ur, u, current_p);
        if(v == Service_Error::VALIDATE_ERROR::SESSION_VERSION_EXPIRED || v == Service_Error::VALIDATE_ERROR::USER_NOT_AVAILABLE || v == Service_Error::VALIDATE_ERROR::VERIFY_FAILED)
        {
            return {Service_Error::SYSTEM_ERROR::NONE, v};
        }
        DB_Helper::begin_transaction(conn.get_hdbc());
        try
        {
            vector<unsigned char> new_salt = Hash::generate_salt(); 
            vector<unsigned char> new_password = Hash::encryption(new_p, new_salt); 
            ur.change_credential(u, new_salt, new_password);
            ur.update_session_version(u.get_user_id());
            Audit_Log::change_password(conn.get_hdbc(), u, "SUCCESS");
            DB_Helper::commit(conn.get_hdbc());
        }
        catch(const Change_Password_Error& e)
        {
            LogService::system_log_exception(conn.get_hdbc(), "Change Password Failed", e.filename_err, e.line_err, e.state, e.native_err, e.what());
            return {Service_Error::SYSTEM_ERROR::CHANGE_PASSWORD_FAILED};
        }
        catch(const Create_Log_Error& e)
        {
            LogService::system_log_exception(conn.get_hdbc(), "Audit Log Failed Detected", e.filename_err, e.line_err, e.state, e.native_err, e.what());
            return {Service_Error::SYSTEM_ERROR::DATABASE_ERROR};
        }
        
    }
    catch(const ConnectionErrors& e)
    {
        System_Log::error_log("DataBase Connection Failed", "system.log", e.filename_err, e.line_err, e.state, e.native_err, e.what());
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

LOGIN AuthService::login(User& u, Account& a, Session& s, string& phone_number, string& password, bool data_user_in_memory) // lỗi ở hàm login audit và check error sai dù nhập đúng
{
    try
    {
        DataBase conn; //Data Base Connection Error
        User_Repository ur(conn.get_hdbc());
        Account_Repository ar(conn.get_hdbc());
        try
        {   
            if(data_user_in_memory == true)
            {
                s.set_user_id(u.get_user_id());
                s.set_session_version(ur.get_version_db(s));
                u.set_ip_address(IP_Manager::get_ip_from_file(u.get_phone_number()));
                Service_Error::VALIDATE_ERROR v = login_verification(u, password);
                if(v == Service_Error::VALIDATE_ERROR::USER_NOT_AVAILABLE)
                {
                    LogService::safe_audit_user_not_available_log(conn.get_hdbc(), u);
                    return {Service_Error::SYSTEM_ERROR::NONE, Service_Error::BUSSINESS_ERROR::NONE, v};
                }
                else if(v == Service_Error::VALIDATE_ERROR::VERIFY_FAILED)
                {
                    LogService::safe_audit_login_log(conn.get_hdbc(), u, a, "WRONG_PASSWORD", "FAILED");
                    return {Service_Error::SYSTEM_ERROR::NONE, Service_Error::BUSSINESS_ERROR::NONE, v};
                }

            }
            else
            {
                Service_Error::BUSSINESS_ERROR lad = load_authenticated_user(u, s, ur, phone_number); 
                if(lad == Service_Error::BUSSINESS_ERROR::USER_NOT_EXIST)
                {
                    u.set_ip_address(Random_Generation::generate_ip());
                    LogService::safe_audit_user_not_found_log(conn.get_hdbc(), u);
                    return {Service_Error::SYSTEM_ERROR::NONE, lad, Service_Error::VALIDATE_ERROR::NONE};
                }
                u.set_phone_number(phone_number);
                u.set_ip_address(IP_Manager::get_ip_from_file(phone_number));
                s.set_user_id(u.get_user_id());
                a.set_user_id(u.get_user_id());
                //get data account
                ar.get_account_information(a); 
                Service_Error::VALIDATE_ERROR v = login_verification(u, password);
                if(v == Service_Error::VALIDATE_ERROR::NONE)
                {
                    if(!check_session_version(s, ur.get_version_db(s)))
                    {
                        LogService::safe_audit_login_log(conn.get_hdbc(), u, a, "SESSION_VERSION_EXPIRED", "FAILED");
                        return {Service_Error::SYSTEM_ERROR::NONE, Service_Error::BUSSINESS_ERROR::NONE, Service_Error::VALIDATE_ERROR::SESSION_VERSION_EXPIRED};
                    }
                    
                }
                else if(v == Service_Error::VALIDATE_ERROR::USER_NOT_AVAILABLE)
                {
                    
                    LogService::safe_audit_user_not_available_log(conn.get_hdbc(), u);  
                    return {Service_Error::SYSTEM_ERROR::NONE, Service_Error::BUSSINESS_ERROR::NONE, v};
                }
                else if(v == Service_Error::VALIDATE_ERROR::VERIFY_FAILED)
                {
                    LogService::safe_audit_login_log(conn.get_hdbc(), u, a, "WRONG_PASSWORD", "FAILED");
                    return {Service_Error::SYSTEM_ERROR::NONE, Service_Error::BUSSINESS_ERROR::NONE, v};
                }
                   
            }
            Audit_Log::login(conn.get_hdbc(), u, a, "LOGIN_SUCCESSFUL", "SUCCESS");
        }
        catch(const Get_Data_Error& e)
        {
            System_Log::error_log("Get Data From DataBase Failed", "system.log",  e.filename_err, e.line_err, e.state, e.native_err, e.what());
            return {Service_Error::SYSTEM_ERROR::FETCH_FAILED};
        }
        catch(const Data_Error& e)
        {
            System_Log::error_log("Null Data Detected", "system.log",  e.filename_err, e.line_err, e.state, e.native_err, e.what());
            return {Service_Error::SYSTEM_ERROR::DATABASE_ERROR};
        }
        catch(const Open_File_Error& e)
        {
            System_Log::error_log("Get Data From File Failed", "system.log",  e.filename_err, e.line_err, e.what());
            return {Service_Error::SYSTEM_ERROR::DATABASE_ERROR};
        }
        catch(const Invalid_Input& e) //get data error 
        {
            System_Log::error_log("Invalid Data Detected", "system.log",  e.filename_err, e.line_err, e.what());
            return {Service_Error::SYSTEM_ERROR::DATABASE_ERROR};
        }
        catch(const Create_Log_Error& e)
        {
            System_Log::error_log("Create Log Failed", "system.log",  e.filename_err, e.line_err, e.state, e.native_err, e.what());
            return {Service_Error::SYSTEM_ERROR::DATABASE_ERROR};
        }
        catch(const Hash_Algorithm_Error& e)
        {
            System_Log::error_log("Login Hash Failed", "system.log",  e.filename_err, e.line_err, e.what());
            return {Service_Error::SYSTEM_ERROR::HASH_FAILED};
        }
    }
    catch(const ConnectionErrors& e)
    {
        System_Log::error_log("DataBase Connection Failed", "system.log",  e.filename_err, e.line_err, e.state, e.native_err, e.what());
        return {Service_Error::SYSTEM_ERROR::CONNECTION_FAILED};
    }
    return {};
}

REGISTER AuthService::registry(User& u, Account& a)
{
    try
    {
        DataBase conn;
        DB_Helper::begin_transaction(conn.get_hdbc());
        try
        {
            User_Repository ur(conn.get_hdbc());
            if(ur.add_new_user(u) == Repository_Error::BUSSINESS_ERROR::DUPLICATE_PHONE_NO)
            {
                DB_Helper::rollback(conn.get_hdbc());
                return {Service_Error::SYSTEM_ERROR::NONE, Service_Error::BUSSINESS_ERROR::E_DUPLICATE_PHONE_NO};
            }
            u.set_ip_address(Random_Generation::generate_ip());
            Audit_Log::user_register(conn.get_hdbc(), u, "USER_CREATED", "SUCCESS");
            a.set_user_id(u.get_user_id());
            //create account
            constexpr int MAX_RETRY = 5;
            Account_Repository ar(conn.get_hdbc());
            for(int attempt = 0; attempt < MAX_RETRY; attempt++)
            {
                a.set_account_no(Random_Generation::generate_account_no(ar));
                Repository_Error::BUSSINESS_ERROR repr = ar.add_account(a);
                if(repr == Repository_Error::BUSSINESS_ERROR::NONE)
                {
                    break;
                }
                if(attempt == (MAX_RETRY - 1) && repr == Repository_Error::BUSSINESS_ERROR::DUPLICATE_ACCOUNT_NO)
                {
                    return {Service_Error::SYSTEM_ERROR::NONE, Service_Error::BUSSINESS_ERROR::E_DUPLICATE_ACCOUNT_NO};                  
                }
            }
            Audit_Log::account_register(conn.get_hdbc(), u, a, "ACCOUNT_CREATED", "SUCCESS");
            DB_Helper::commit(conn.get_hdbc());
        }
        catch(const Create_New_User_Error& e)
        {
            LogService::system_log_exception(conn.get_hdbc(), "Create New User Failed", e.filename_err, e.line_err, e.state, e.native_err, e.what());
            return {Service_Error::SYSTEM_ERROR::CREATE_USER_FAILED};
        }
        catch(const Create_New_Account_Error& e)
        {
            LogService::system_log_exception(conn.get_hdbc(), "Create New Account Failed", e.filename_err, e.line_err, e.state, e.native_err, e.what());
            return {Service_Error::SYSTEM_ERROR::CREATE_ACCOUNT_FAILED};
        }
        catch(const Open_File_Error& e)
        {
            if(string(e.what()) == "Can't registration ip to this file: 'ip_profile.txt'.")
            {
                System_Log::error_log("Registration New IP Failed", "system.log",  e.filename_err, e.line_err, e.what());
                return {};
            }
            System_Log::error_log("Get Data From File Failed", "system.log",  e.filename_err, e.line_err, e.what());
            return {Service_Error::SYSTEM_ERROR::DATABASE_ERROR};
        }
        catch(const Invalid_Input& e)
        {
            System_Log::error_log("Invalid Data Detected", "system.log",  e.filename_err, e.line_err, e.what());
            return {Service_Error::SYSTEM_ERROR::DATABASE_ERROR};
        }
        catch(const Hash_Algorithm_Error& e)
        {
            LogService::system_log_exception(conn.get_hdbc(), "Register Hash Failed", e.filename_err, e.line_err, e.what());
            return {Service_Error::SYSTEM_ERROR::HASH_FAILED};
            
        }
        catch(const Create_Log_Error& e)
        {
            LogService::system_log_exception(conn.get_hdbc(), "Audit Log Failed Detected", e.filename_err, e.line_err, e.state, e.native_err, e.what());
            return {Service_Error::SYSTEM_ERROR::DATABASE_ERROR};
        }
        catch(const Get_Data_Error& e)
        {
            LogService::system_log_exception(conn.get_hdbc(), "Get Data Failed", e.filename_err, e.line_err, e.state, e.native_err, e.what());
            return {Service_Error::SYSTEM_ERROR::FETCH_FAILED};  
        }
        IP_Manager::ip_registration(u.get_phone_number(), u.get_ip_address());

    }
    catch(const ConnectionErrors& e)
    {
        System_Log::error_log("DataBase Connection Failed", "system.log",  e.filename_err, e.line_err, e.state, e.native_err, e.what());
        return {Service_Error::SYSTEM_ERROR::CONNECTION_FAILED};
    }
    return {};
}

LOGOUT AuthService::logout(User& u, Account& a, bool enforced_logout)
{
    try
    {
        DataBase conn; //Data Base Connection Error
        if(enforced_logout)
        {
            Audit_Log::logout(conn.get_hdbc(), u, a, "SESSION_VERSION_EXPIRED", "SUCCESS");
        }
        else
        {
            Audit_Log::logout(conn.get_hdbc(), u, a, "USER_LOGGED_OUT", "SUCCESS");
        }
    }
    catch(const ConnectionErrors& e)
    {
        System_Log::error_log("DataBase Connection Failed", "system.log",  e.filename_err, e.line_err, e.state, e.native_err, e.what());
        return {Service_Error::SYSTEM_ERROR::CONNECTION_FAILED};
    }
    catch(const Create_Log_Error& e)
    {
        System_Log::error_log("Create Log Failed", "system.log",  e.filename_err, e.line_err, e.state, e.native_err, e.what());
        return {Service_Error::SYSTEM_ERROR::DATABASE_ERROR};
    }
    return {};
    
}
