#include "audit_log.h"
#include "Shared/error/database_errors.h"
#include "Shared/utils/odbc_helper_error.h"
#include "Shared/data/sql_error.h"
#include "Shared/utils/db_helper.h"
#include "Shared/utils/file_handle.h"
#include "utils/repository_error_handle.h"
using namespace std;

void Audit_Log::change_password(SQLHDBC hdbc, User& u, const string& status_activity)
{
    SQLRETURN res;

    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "INSERT INTO Activity_Logs(Activity_Type, Status_Activity, Detail_Event, UserID, Target_User_ID) VALUES (?,?,?,?,?)";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            int uid = u.get_user_id();
            string type = "CHANGE_PASSWORD";
            string detail = "PASSWORD_CHANGED_SUCCESSFULLY";

            SQLLEN Typelen = SQL_NTS;
            SQLLEN Statuslen = SQL_NTS;
            SQLLEN Detail_len = SQL_NTS;
            
            DB_Helper::bind_parameter_string(hstmt, 1, Typelen, type);
            DB_Helper::bind_parameter_string(hstmt, 2, Statuslen, status_activity);
            DB_Helper::bind_parameter_string(hstmt, 3, Detail_len, detail);
            DB_Helper::bind_parameter_int(hstmt, 4, uid);
            DB_Helper::bind_parameter_int(hstmt, 5, uid);

            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
            }
        }
        else
        {
            Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }
        
    }
    else
    {
        Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
}

void Audit_Log::change_status_user(SQLHDBC hdbc, User& u, Status_User_Info& sui, const string& status_activity)
{
    SQLRETURN res;

    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "INSERT INTO Activity_Logs(Activity_Type, Status_Activity, Detail_Event, UserID, Target_User_ID) VALUES (?,?,?,?,?)";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            int uid = u.get_user_id();
            string type = "CHANGE_STATUS_USER";
            string detail = string(sui.current_status) + " -> " + string(sui.new_status);

            SQLLEN Typelen = SQL_NTS;
            SQLLEN Statuslen = SQL_NTS;
            SQLLEN Detail_len = SQL_NTS;
            
            DB_Helper::bind_parameter_string(hstmt, 1, Typelen, type);
            DB_Helper::bind_parameter_string(hstmt, 2, Statuslen, status_activity);
            DB_Helper::bind_parameter_string(hstmt, 3, Detail_len, detail);
            DB_Helper::bind_parameter_int(hstmt, 4, uid);
            DB_Helper::bind_parameter_int(hstmt, 5, sui.userid); //check status dest_account

            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
            }

        }

        else
        {
            Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }
        

    }
    else
    {
        Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
}

void Audit_Log::change_status_account(SQLHDBC hdbc, User& u, Status_Account_Info& sai, const string& status_activity)
{    
    SQLRETURN res;

    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "INSERT INTO Activity_Logs(Activity_Type, Status_Activity, Detail_Event, UserID, Target_Account_ID) VALUES (?,?,?,?,?)";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            int uid = u.get_user_id();
            string type = "CHANGE_STATUS_ACCOUNT";
            string detail = string(sai.current_status) + " -> " + string(sai.new_status);

            SQLLEN Typelen = SQL_NTS;
            SQLLEN Statuslen = SQL_NTS;
            SQLLEN Detail_len = SQL_NTS;
            
            DB_Helper::bind_parameter_string(hstmt, 1, Typelen, type);
            DB_Helper::bind_parameter_string(hstmt, 2, Statuslen, status_activity);
            DB_Helper::bind_parameter_string(hstmt, 3, Detail_len, detail);
            DB_Helper::bind_parameter_int(hstmt, 4, uid);
            DB_Helper::bind_parameter_int(hstmt, 5, sai.accountid); //check status dest_account

            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
            }
        }

        else
        {
            Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }
        

    }
    else
    {
        Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
}

Repository_Error::BUSSINESS_ERROR Audit_Log::create_transaction_audit(SQLHDBC hdbc, Account& a, Transaction_Info& t, const string& status)
{
    SQLRETURN res;

    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "INSERT INTO Transactions(Transaction_Code, Type_Transaction, From_Account_ID, To_Account_ID, Transaction_Amount, Status_Transaction) VALUES(?,?,?,?,?,?)";
        res = SQLPrepareA(hstmt, query, SQL_NTS);
        if(SQL_SUCCEEDED(res))
        {
            string type = "TRANSFER";
            int aid = a.get_account_id();

            SQLLEN Typelen = SQL_NTS;
            SQLLEN Statuslen = SQL_NTS;
            SQLLEN Codelen = SQL_NTS;
            
            DB_Helper::bind_parameter_string(hstmt, 1, Codelen, t.transaction_code);
            DB_Helper::bind_parameter_string(hstmt, 2, Typelen, type);
            DB_Helper::bind_parameter_int(hstmt, 3, aid);
            DB_Helper::bind_parameter_int(hstmt, 4, t.dest_account_id); //check status dest_account
            DB_Helper::bind_parameter_double(hstmt, 5, t.amount);
            DB_Helper::bind_parameter_string(hstmt, 6, Statuslen, status);

            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
                return Repository_Error::BUSSINESS_ERROR::DUPLICATE_TRANSACTION_NO;
            }
        }
        else
        {
            Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }
        
    }
    else
    {
        Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return Repository_Error::BUSSINESS_ERROR::NONE;
    
}

void Audit_Log::user_not_found(SQLHDBC hdbc, User& u)
{
    SQLRETURN res;

    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "INSERT INTO Auth_Logs(Action_Log, Status_Log, Detail, IP_Address) VALUES(?,?,?,?)";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            string action = "LOGIN";
            string status = "FAILED";
            string message = "USER_NOT_FOUND";
            string ip = u.get_ip_address();

            SQLLEN Actionlen = SQL_NTS;
            SQLLEN Statuslen = SQL_NTS;
            SQLLEN Messagelen = SQL_NTS;
            SQLLEN Iplen = SQL_NTS;
 
            DB_Helper::bind_parameter_string(hstmt, 1, Actionlen, action);
            DB_Helper::bind_parameter_string(hstmt, 2, Statuslen, status);
            DB_Helper::bind_parameter_string(hstmt, 3, Messagelen, message);
            DB_Helper::bind_parameter_string(hstmt, 4, Iplen, ip);


            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);

            }

        }
        else
        {
            Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }
    
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
}


void Audit_Log::user_not_available(SQLHDBC hdbc, User& u)
{
    SQLRETURN res;

    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "INSERT INTO Auth_Logs(Action_Log, Status_Log, Detail, UserID, IP_Address) VALUES(?,?,?,?,?)";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            string message;

            if(u.get_status_user() == "CLOSED")
            {
                message = "USER_CLOSED";
            }
            else if(u.get_status_user() == "LOCKED")
            {
                message = "USER_LOCKED";
            }
            else if(u.get_status_user() == "SUSPENDED")
            {
                message = "USER_SUSPENDED";
            }

            string action = "LOGIN";
            string status = "DENIED";
            int uid = u.get_user_id();
            string ip = u.get_ip_address();

            SQLLEN Actionlen = SQL_NTS;
            SQLLEN Messagelen = SQL_NTS;
            SQLLEN Statuslen = SQL_NTS;
            SQLLEN Iplen = SQL_NTS;


            DB_Helper::bind_parameter_string(hstmt, 1, Actionlen, action);
            DB_Helper::bind_parameter_string(hstmt, 2, Statuslen, status);
            DB_Helper::bind_parameter_string(hstmt, 3, Messagelen, message);
            DB_Helper::bind_parameter_int(hstmt, 4, uid);
            DB_Helper::bind_parameter_string(hstmt, 5, Iplen, ip);

            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);

            }

        }

        else
        {
            Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }
        

    }
    else
    {
        Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);     
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

}


void Audit_Log::login(SQLHDBC hdbc, User& u, const string& message, const string& status) 
{
    
    SQLRETURN res;

    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "INSERT INTO Auth_Logs(Action_Log, Status_Log, Detail, UserID, IP_Address) VALUES(?,?,?,?,?)";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            int uid = u.get_user_id();
            string action = "LOGIN";
            string ip = u.get_ip_address();

            SQLLEN Actionlen = SQL_NTS;
            SQLLEN Statuslen = SQL_NTS;
            SQLLEN Messagelen = SQL_NTS;
            SQLLEN Iplen = SQL_NTS;
            
        
            DB_Helper::bind_parameter_string(hstmt, 1, Actionlen, action);
            DB_Helper::bind_parameter_string(hstmt, 2, Statuslen, status);
            DB_Helper::bind_parameter_string(hstmt, 3, Messagelen, message);
            DB_Helper::bind_parameter_int(hstmt, 4, uid);
            DB_Helper::bind_parameter_string(hstmt, 5, Iplen, ip);

            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);

            }

        }
        else
        {
            Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }
    
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

}

void Audit_Log::login(SQLHDBC hdbc, User& u, Account& a, const string& message, const string& status)
{
    
    SQLRETURN res;

    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "INSERT INTO Auth_Logs(Action_Log, Status_Log, Detail, UserID, Target_Account_ID, IP_Address) VALUES(?,?,?,?,?,?)";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            string action = "LOGIN";
            string ip = u.get_ip_address();

            SQLLEN Actionlen = SQL_NTS;
            SQLLEN Statuslen = SQL_NTS;
            SQLLEN Messagelen = SQL_NTS;
            SQLLEN Iplen = SQL_NTS;
 
            int uid = a.get_user_id();
            int aid = a.get_account_id();

            DB_Helper::bind_parameter_string(hstmt, 1, Actionlen, action);
            DB_Helper::bind_parameter_string(hstmt, 2, Statuslen, status);
            DB_Helper::bind_parameter_string(hstmt, 3, Messagelen, message);
            DB_Helper::bind_parameter_int(hstmt, 4, uid);
            DB_Helper::bind_parameter_int(hstmt, 5, aid);
            DB_Helper::bind_parameter_string(hstmt, 6, Iplen, ip);

            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);

            }

        }
        else
        {
            Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }
    
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

}

void Audit_Log::account_register(SQLHDBC hdbc, User& u, Account& a, const string& message, const string& status)
{
    SQLRETURN res;

    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "INSERT INTO Auth_Logs(Action_Log, Status_Log, Detail, UserID, Target_Account_ID, IP_Address) VALUES(?,?,?,?,?,?)"; //IP address có thể dùng C++ để query lên Server
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            string action = "REGISTER";
            string ip = u.get_ip_address();

            SQLLEN Actionlen = SQL_NTS;
            SQLLEN Statuslen = SQL_NTS;
            SQLLEN Messagelen = SQL_NTS;
            SQLLEN Iplen = SQL_NTS;

            int uid = a.get_user_id();
            int aid = a.get_account_id();

            DB_Helper::bind_parameter_string(hstmt, 1, Actionlen, action);
            DB_Helper::bind_parameter_string(hstmt, 2, Statuslen, status);
            DB_Helper::bind_parameter_string(hstmt, 3, Messagelen, message);
            DB_Helper::bind_parameter_int(hstmt, 4, uid);
            DB_Helper::bind_parameter_int(hstmt, 5, aid);
            DB_Helper::bind_parameter_string(hstmt, 6, Iplen, ip);

            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
            }

        }
        else
        {
            Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }
    
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
}

void Audit_Log::user_register(SQLHDBC hdbc, User& u, const string& message, const string& status)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "INSERT INTO Auth_Logs(Action_Log, Status_Log, Detail, UserID, IP_Address) VALUES(?,?,?,?,?)"; //IP address có thể dùng C++ để query lên Server
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            string action = "REGISTER";
            string ip = u.get_ip_address();

            SQLLEN Actionlen = SQL_NTS;
            SQLLEN Statuslen = SQL_NTS;
            SQLLEN Messagelen = SQL_NTS; 
            SQLLEN Iplen = SQL_NTS;

            string s = status;
            string m = message;
            int uid = u.get_user_id();

            DB_Helper::bind_parameter_string(hstmt, 1, Actionlen, action);
            DB_Helper::bind_parameter_string(hstmt, 2, Statuslen, s);
            DB_Helper::bind_parameter_string(hstmt, 3, Messagelen, m);
            DB_Helper::bind_parameter_int(hstmt, 4, uid);
            DB_Helper::bind_parameter_string(hstmt, 5, Iplen, ip);

            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
            }
        }
        else
        {
            Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

}

void Audit_Log::logout(SQLHDBC hdbc, User& u, Account& a, const string& message, const string& status)
{
    SQLRETURN res;

    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "INSERT INTO Auth_Logs(Action_Log, Status_Log, Detail, UserID, Target_Account_ID, IP_Address) VALUES(?,?,?,?,?,?)";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            string action = "LOGOUT";
            string ip = u.get_ip_address();

            SQLLEN Actionlen = SQL_NTS;
            SQLLEN Statuslen = SQL_NTS;
            SQLLEN Messagelen = SQL_NTS;
            SQLLEN Iplen = SQL_NTS;
 
            int uid = a.get_user_id();
            int aid = a.get_account_id();
            DB_Helper::bind_parameter_string(hstmt, 1, Actionlen, action);
            DB_Helper::bind_parameter_string(hstmt, 2, Statuslen, status);
            DB_Helper::bind_parameter_string(hstmt, 3, Messagelen, message);
            DB_Helper::bind_parameter_int(hstmt, 4, uid);
            DB_Helper::bind_parameter_int(hstmt, 5, aid);
            DB_Helper::bind_parameter_string(hstmt, 6, Iplen, ip);

            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {

                Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);

            }

        }
        else
        {
            Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Repository_Handle::handle_error<Create_Log_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }
    
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
}
