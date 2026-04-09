#include "auth_service.h"
#include <stdexcept>
#include <vector>
#include "db_helper.h"
#include "system_log.h"
#include "hash.h"
#include "random_generation.h"
#include "database_errors.h"
#include "system_errors.h"
#include "audit_log.h"
#include "account_service.h"
#include "validation_helper.h"
#include "helper_support.h"
#include "session.h"
using namespace std;

// bool AuthService::check_permission_dest_account_before_create_transaction(User& u)
// {

// }


AuthService::VERIFY AuthService::authentication(User& u, Account& a, string& password)
{
    if(Validate::verify(u, password) == true)
    {
        // cout << "On verify success\n";
        if(!Validate::check_status_user(u))
        {
            // cout << "on check status fail!\n";
            return VERIFY::USER_NOT_AVAILABLE;
        }
        return VERIFY::SUCCESS;
    }
    return VERIFY::WRONG_PASSWORD;
    
}

AuthService::GET_ACCOUNT AuthService::get_account_information(Account& a, Account_Repository& ar)
{
    // cout << "Before get data account\n";
    Account_Repository::Repository_Result repr = ar.get_information_account(a); //giá trị vô nghĩa
    // cout << "After get data account\n";
    if(repr == Account_Repository::Repository_Result::ACCOUNT_NOT_FOUND)
    {
        // cout << "On account not found\n";
        return GET_ACCOUNT::ACCOUNT_NOT_FOUND;
    }
    return GET_ACCOUNT::SUCCESS;
    
}

AuthService::GET_USER AuthService::get_user_information(User& u, Session& s, User_Repository& ur, string& phone_number)
{
    User_Repository::Repository_Result repr = ur.get_user_information(u, s, phone_number);

    if(repr == User_Repository::Repository_Result::PHONE_NUMBER_DOES_NOT_EXIST)
    {
        return GET_USER::PHONE_NUMBER_DOES_NOT_EXIST;
    }

    ur.get_user_role(u);

    return GET_USER::SUCCESS;
    
}

AuthService::LOGIN AuthService::login(User& u, Account& a, Session& s, string& phone_number, string& password, bool data_user_in_memory) // lỗi ở hàm login audit và check error sai dù nhập đúng
{
    DataBase conn; //Data Base Connection Error
    User_Repository ur(conn.get_hdbc());
    Account_Repository ar(conn.get_hdbc());
    try
    {
        if(data_user_in_memory == true)
        {
            cout << "Already in data true\n";
            AuthService::VERIFY v = authentication(u, a, password);
            if(v == AuthService::VERIFY::USER_NOT_AVAILABLE)
            {
                Audit_Log::user_not_available(conn.get_hdbc(), u);
                return LOGIN::USER_NOT_AVAILABLE;
            }
            else if(v == AuthService::VERIFY::WRONG_PASSWORD)
            {
                Audit_Log::login(conn.get_hdbc(), a, "WRONG_PASSWORD", "FAILED");
                return LOGIN::VERIFY_FAILED;
            }
            ur.get_session_version(u, s); 

        }
        else
        {
            // cout << "Already in data false\n";

            //query trước để lấy salt, userid, phonenumber, password hash.
            AuthService::GET_USER gu = get_user_information(u, s, ur, phone_number);
            cout << "Done Get user\n";
            if(gu == AuthService::GET_USER::PHONE_NUMBER_DOES_NOT_EXIST)
            {
                cout << "Already Get PNDNE\n";
                Audit_Log::user_not_found(conn.get_hdbc(), u);
                return LOGIN::USER_DOES_NOT_EXIST;
            }
            else if(gu == AuthService::GET_USER::SUCCESS)
            {
                // cout << "SUCCESS\n" ;
                a.user_id = u.user_id;
                AuthService::VERIFY v = authentication(u, a, password);

                if(v == AuthService::VERIFY::SUCCESS)
                {
                    cout << "Get user Success\n";
                    // get information account
                    AuthService::GET_ACCOUNT ga = get_account_information(a, ar);

                    if(ga == AuthService::GET_ACCOUNT::ACCOUNT_NOT_FOUND)
                    {
                        Audit_Log::login(conn.get_hdbc(), u, "ACCOUNT_NOT_FOUND", "FAILED");
                        return LOGIN::ACCOUNT_NOT_FOUND;
                    }

                }
                else if(v == AuthService::VERIFY::USER_NOT_AVAILABLE)
                {

                    Audit_Log::user_not_available(conn.get_hdbc(), u);
                    return LOGIN::USER_NOT_AVAILABLE;
                }
                else if(v == AuthService::VERIFY::WRONG_PASSWORD)
                {
                    cout << "WRONG PASSS\n";

                    Audit_Log::login(conn.get_hdbc(), a, "WRONG_PASSWORD", "FAILED");
                    return LOGIN::VERIFY_FAILED;
                }

            }
        }
        

    }
    catch(const Get_User_Error& e)
    {
        cout << "Get_User_Error\n";
        try
        {
            Audit_Log::login(conn.get_hdbc(), u, "SYSTEM_ERROR", "FAILED");
        }
        catch(const Create_Log_Error& e)
        {
            System_Log::error_log("Audit Log Failed Detected", e.filename_err, e.what());
        }
        System_Log::error_log("Get Data Of User From DataBase Failed", e.filename_err, e.state, e.native_err, e.what());
        return LOGIN::DATABASE_ERROR;

    }
    catch(const Get_Account_Error& e)
    {
        cout << "Get_Account_Error\n";
        try
        {
            Audit_Log::login(conn.get_hdbc(), a, "SYSTEM_ERROR", "FAILED");
        }
        catch(const Create_Log_Error& e)
        {
            System_Log::error_log("Audit Log Failed Detected", e.filename_err, e.what());
        }
        System_Log::error_log("Get Data Of Account From DataBase Failed", e.filename_err, e.state, e.native_err, e.what());
        return LOGIN::DATABASE_ERROR;
    }
    catch(const Get_Data_Error& e) // case null data
    {
        cout << "Get_Data_Error\n";

        try
        {
            Audit_Log::login(conn.get_hdbc(), a, "SYSTEM_ERROR", "FAILED");
        }
        catch(const Create_Log_Error& e)
        {
            System_Log::error_log("Audit Log Failed Detected", e.filename_err, e.what());
        }
        System_Log::error_log("Get Data From DataBase Failed", e.filename_err, e.state, e.native_err, e.what());
        return LOGIN::DATABASE_ERROR;
    }
    catch(const Data_Error& e)
    {
        //check lại vụ null data
        System_Log::error_log("Null Data Detected", e.filename_err, e.state, e.native_err, e.what());
        return LOGIN::DATABASE_ERROR;
    }
    catch(const Create_Log_Error& e)
    {
        System_Log::error_log("Create Log Failed", e.filename_err, e.state, e.native_err, e.what());
        return LOGIN::DATABASE_ERROR; // !!!!return sai
    }
    catch(const Hash_Algorithm_Error& e)
    {
        System_Log::error_log("Login Hash Failed", e.filename_err, e.what());
        return LOGIN::HASH_FAILED;
    }

    //catch các ngoại lệ bắt buộc same in registry
    try
    {
        Audit_Log::login(conn.get_hdbc(), a, "LOGIN_SUCCESSFUL", "SUCCESS");
    }
    catch(const Create_Log_Error& e)
    {
        System_Log::error_log("Audit Log Failed Detected", e.filename_err, e.what());
    }

    return LOGIN::SUCCESS;
    
}

void AuthService::handle_duplicate_account_no(DataBase& conn, Account& a, Account_Repository& ar, Account_Repository::Repository_Result repr)
{
    a.set_account_no(create_account_no(a, ar));
    check_error(conn, u, a, ar, ar.add_account(u, a));
}

void AuthService::check_error(DataBase& conn, Account& a, Account_Repository& ar, Account_Repository::Repository_Result repr)
{
    switch(repr)
    {
        case Account_Repository::Repository_Result::SUCCESS:
        {
            break;

        }
        case Account_Repository::Repository_Result::DUPLICATE_ACCOUNT_NO:
        {
            handle_duplicate_account_no(conn, a, ar, repr);
            break;
        }
    }

}

AuthService::REGISTER AuthService::registry(User& u, Account& a)
{
    //insert data to data base
    try
    {
        DataBase conn; //Data Base Connection Error
        conn.set_off_auto_commit();

        //Create obj
        User_Repository ur(conn.get_hdbc());
        Account_Repository ar(conn.get_hdbc());

        //begin transaction
        DB_Helper::begin_transaction(conn.get_hdbc());

        try
        {
            a.set_account_no(create_account_no(a, ar));
            if(ur.add_new_user(u) == User_Repository::Repository_Result::DUPLICATE_PHONE_NO)
            {
                DB_Helper::rollback(conn.get_hdbc());
                return REGISTER::E_DUPLICATE_PHONE_NO;
            }

            // cout << "check_err done\n";
            Audit_Log::user_register(conn.get_hdbc(), u, "USER_CREATED", "SUCCESS");
            a.set_user_id(u.get_user_id());
            // cout << "done create user\n";

            //create account
            check_error(conn, a, ar, ar.add_account(a)); 
            Audit_Log::account_register(conn.get_hdbc(), a, "ACCOUNT_CREATED", "SUCCESS");

            // cout << "done create account\n";

            //commit
            DB_Helper::commit(conn.get_hdbc());
            
            // cout << "done Commit\n";

        }
        catch(const Create_New_User_Error& e)
        {
            cout << "Create_New_User_Error\n";

            DB_Helper::rollback(conn.get_hdbc());
            System_Log::error_log("Create New User Failed", e.filename_err, e.state, e.native_err, e.what());
            return REGISTER::CREATE_USER_FAILED;
        }
        catch(const Create_New_Account_Error& e)
        {
            cout << "Create_New_Account_Error\n";

            DB_Helper::rollback(conn.get_hdbc());
            System_Log::error_log("Create New Account Failed", e.filename_err, e.state, e.native_err, e.what());
            return REGISTER::CREATE_ACCOUNT_FAILED;
        }
        catch(const Hash_Algorithm_Error& e)
        {
            DB_Helper::rollback(conn.get_hdbc());
            System_Log::error_log("Register Hash Failed", e.filename_err, e.what());
            return REGISTER::HASH_FAILED;
        }
        catch(const Create_Log_Error& e)
        {
            System_Log::error_log("Audit Log Failed Detected", e.filename_err, e.state, e.native_err, e.what());
            return REGISTER::SUCCESS;
        }
        catch(const Get_Data_Error& e)
        {
            DB_Helper::rollback(conn.get_hdbc());
            System_Log::error_log("Get Data Failed", e.filename_err, e.state, e.native_err, e.what());
            return REGISTER::FETCH_FAILED;
        }
        
    }
    catch(const ConnectionErrors& e)
    {
        cout << "Connection_Error\n";
        System_Log::error_log("DataBase Connection Failed", e.filename_err, e.state, e.native_err, e.what());
        return REGISTER::CONNECTION_FAILED;
    }
    cout << "success\n";

    return REGISTER::SUCCESS;
   
}
