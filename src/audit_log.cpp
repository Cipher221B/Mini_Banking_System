#include "audit_log.h"
#include "database_errors.h"
#include "odbc_helper_error.h"
#include "sql_error.h"
#include "db_helper.h"
#include "helper_support.h"

void Audit_Log::handle_error(SQLSMALLINT type, SQLHANDLE handle)
{
    Error_Info last_err = Helper_Error::get_infor_error(type, handle);
    if(last_err.native_err == 2627 || last_err.native_err == 2601) //DUPLICATE ACCOUNT
    {
        SQLFreeHandle(type, handle);
    }
    else
    {
        SQLFreeHandle(type, handle);
        throw Create_Log_Error(last_err.message_err, __FILE__, last_err.sql_state, last_err.native_err);
    }
    
}


void Audit_Log::handle_error(SQLSMALLINT type_err, SQLHANDLE handle_err, SQLSMALLINT type_free, SQLHANDLE handle_free)
{
    Error_Info last_err = Helper_Error::get_infor_error(type_err, handle_err);
    SQLFreeHandle(type_free, handle_free);
    throw Create_Log_Error(last_err.message_err, __FILE__, last_err.sql_state, last_err.native_err);

}

// void Audit_Log::create_user_success(SQLHDBC hdbc, User& u) //User_Activity
// {
//     SQLRETURN res;

//     SQLHSTMT hstmt = SQL_NULL_HSTMT;
//     res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

//     if(SQL_SUCCEEDED(res))
//     {
//         SQLCHAR query[] = "INSERT INTO User_Activity(Detail_Event, Activity_Type, UserID, Status_Activity) VALUES(?,?,?,?)";
//         res = SQLPrepare(hstmt, query, SQL_NTS);

//         if(SQL_SUCCEEDED(res))
//         {
//             string message = "Create User Success";
//             wstring wm(message.begin(), message.end());

//             string type = "CREATE";
//             string status = "SUCCESS";

//             SQLLEN Messagelen = wm.size()  * sizeof(wchar_t); // 1 wchar = 2 byte if use wm.size() = 19 but odbc understand = 9 wchar so it will loss data 
//             SQLLEN Activitylen = type.size();
//             SQLLEN Statuslen = status.size();

//             SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, Messagelen, 0, (SQLPOINTER)wm.c_str(), Messagelen, &Messagelen); // bind for NVARCHAR
//             SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, Activitylen, 0, (SQLPOINTER)type.c_str(), Activitylen, &Activitylen); //bind for VARCHAR
//             SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &u.user_id, 0, NULL); // bind for INT
//             SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, Statuslen, 0, (SQLPOINTER)status.c_str(), Statuslen, &Statuslen);

//             res = SQLExecute(hstmt);

//             if(!SQL_SUCCEEDED(res))
//             {
//                 handle_error(SQL_HANDLE_STMT, hstmt);

//             }

//         }
//         else
//         {
//             handle_error(SQL_HANDLE_STMT, hstmt);
//         }

//     }
//     else
//     {
//         handle_error(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
//     }
    
//     SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

// }

// void Audit_Log::create_account_success(SQLHDBC hdbc, User& u, Account& a) //User_Activity
// {
//     SQLRETURN res;

//     SQLHSTMT hstmt = SQL_NULL_HSTMT;
//     res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

//     if(SQL_SUCCEEDED(res))
//     {
//         SQLCHAR query[] = "INSERT INTO User_Activity(Detail_Event, Activity_Type, UserID, Target_Account_ID, Status_Activity) VALUES(?,?,?,?,?)";
//         res = SQLPrepare(hstmt, query, SQL_NTS);

//         if(SQL_SUCCEEDED(res))
//         {
//             string message = "Create Account Success";
//             wstring wm(message.begin(), message.end());

//             string type = "CREATE";
//             string status = "SUCCESS";

//             SQLLEN Messagelen = wm.size()  * sizeof(wchar_t);
//             SQLLEN Activitylen = type.size();
//             SQLLEN Statuslen = status.size();

//             SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, Messagelen, 0, (SQLPOINTER)wm.c_str(), Messagelen, &Messagelen);
//             SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, Activitylen, 0, (SQLPOINTER)type.c_str(), Activitylen, &Activitylen);
//             SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &u.user_id, 0, NULL);
//             SQLBindParameter(hstmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &a.account_id, 0, NULL);
//             SQLBindParameter(hstmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, Statuslen, 0, (SQLPOINTER)status.c_str(), Statuslen, &Statuslen);
            
//             res = SQLExecute(hstmt);
            
//             if(!SQL_SUCCEEDED(res))
//             {
//                 cout << "to execute\n";
//                 handle_error(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
//             }

//         }
//         else
//         {
//             handle_error(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
//         }

//     }
//     else
//     {
//         handle_error(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
//     }
    
    

//     SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

// }

void Audit_Log::user_register(SQLHDBC hdbc, User& u, const string& message, const string& status)
{
    SQLRETURN res;

    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "INSERT INTO Auth_Logs(Action_Log, Status_Log, Detail, UserID) VALUES(?,?,?,?)"; //IP address có thể dùng C++ để query lên Server
        res = SQLPrepare(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            string action = "REGISTER";

            SQLLEN Actionlen, Statuslen, Messagelen; 

            DB_Helper::bind_parameter_string(hstmt, 1, Actionlen, action);
            DB_Helper::bind_parameter_string(hstmt, 2, Statuslen, status);
            DB_Helper::bind_parameter_string(hstmt, 3, Messagelen, message);
            DB_Helper::bind_parameter_int(hstmt, 4, u.user_id);

            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                handle_error(SQL_HANDLE_STMT, hstmt);
            }

        }
        else
        {
            handle_error(SQL_HANDLE_STMT, hstmt);
        }

    }
    else
    {
        handle_error(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
    }
    
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);


}

void Audit_Log::account_register(SQLHDBC hdbc, Account& a, const string& message, const string& status)
{
    SQLRETURN res;

    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "INSERT INTO Auth_Logs(Action_Log, Status_Log, Detail, UserID, Target_Account_ID) VALUES(?,?,?,?,?)"; //IP address có thể dùng C++ để query lên Server
        res = SQLPrepare(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            string action = "REGISTER";

            SQLLEN Actionlen, Statuslen, Messagelen; 

            DB_Helper::bind_parameter_string(hstmt, 1, Actionlen, action);
            DB_Helper::bind_parameter_string(hstmt, 2, Statuslen, status);
            DB_Helper::bind_parameter_string(hstmt, 3, Messagelen, message);
            DB_Helper::bind_parameter_int(hstmt, 4, a.user_id);
            DB_Helper::bind_parameter_int(hstmt, 5, a.account_id);
            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                handle_error(SQL_HANDLE_STMT, hstmt);
            }

        }
        else
        {
            handle_error(SQL_HANDLE_STMT, hstmt);
        }

    }
    else
    {
        handle_error(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
    }
    
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
}

void Audit_Log::login(SQLHDBC hdbc, Account& a, const string& message, const string& status)
{
    
    SQLRETURN res;

    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "INSERT INTO Auth_Logs(Action_Log, Status_Log, Detail, UserID, Target_Account_ID) VALUES(?,?,?,?,?)";
        res = SQLPrepare(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            // cout << "On success\n";
            string action = "LOGIN";

            SQLLEN Actionlen, Messagelen, Statuslen;

            DB_Helper::bind_parameter_string(hstmt, 1, Actionlen, action);
            DB_Helper::bind_parameter_string(hstmt, 2, Statuslen, status);
            DB_Helper::bind_parameter_string(hstmt, 3, Messagelen, message);
            DB_Helper::bind_parameter_int(hstmt, 4, a.user_id);
            DB_Helper::bind_parameter_int(hstmt, 5, a.account_id);
            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                handle_error(SQL_HANDLE_STMT, hstmt);

            }

        }
        else
        {
            handle_error(SQL_HANDLE_STMT, hstmt);
        }

    }
    else
    {
        handle_error(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
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
        SQLCHAR query[] = "INSERT INTO Auth_Logs(Action_Log, Status_Log, Detail, UserID) VALUES(?,?,?,?)";
        res = SQLPrepare(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {

            string action = "LOGIN";
            
            SQLLEN Actionlen, Messagelen, Statuslen;

            DB_Helper::bind_parameter_string(hstmt, 1, Actionlen, action);
            DB_Helper::bind_parameter_string(hstmt, 2, Statuslen, status);
            DB_Helper::bind_parameter_string(hstmt, 3, Messagelen, message);
            DB_Helper::bind_parameter_int(hstmt, 4, u.user_id);
            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                handle_error(SQL_HANDLE_STMT, hstmt);

            }

        }
        else
        {
            handle_error(SQL_HANDLE_STMT, hstmt);
        }

    }
    else
    {
        handle_error(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
    }
    
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

}

void Audit_Log::user_not_found(SQLHDBC hdbc, User& u)
{
    SQLRETURN res;

    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "INSERT INTO Auth_Logs(Action_Log, Status_Log, Detail) VALUES(?,?,?)";
        res = SQLPrepare(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            string action = "LOGIN";
            string message = "USER_NOT_FOUND";
            string status = "FAILED";
            
            SQLLEN Actionlen, Messagelen, Statuslen;

            DB_Helper::bind_parameter_string(hstmt, 1, Actionlen, action);
            DB_Helper::bind_parameter_string(hstmt, 2, Statuslen, status);
            DB_Helper::bind_parameter_string(hstmt, 3, Messagelen, message);

            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                handle_error(SQL_HANDLE_STMT, hstmt);

            }

        }
        else
        {
            handle_error(SQL_HANDLE_STMT, hstmt);
        }

    }
    else
    {
        handle_error(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
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
        SQLCHAR query[] = "INSERT INTO Auth_Logs(Action_Log, Status_Log, Detail, UserID) VALUES(?,?,?,?)";
        res = SQLPrepare(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            string message;

            if(u.status_user == "CLOSED")
            {
                message = "USER_CLOSED";
            }
            else if(u.status_user == "LOCKED")
            {
                message = "USER_LOCKED";
            }
            else if(u.status_user == "SUSPENDED")
            {
                message = "USER_SUSPENDED";
            }

            string action = "LOGIN";
            string status = "DENIED";

            SQLLEN Actionlen, Messagelen, Statuslen;

            DB_Helper::bind_parameter_string(hstmt, 1, Actionlen, action);
            DB_Helper::bind_parameter_string(hstmt, 2, Statuslen, status);
            DB_Helper::bind_parameter_string(hstmt, 3, Messagelen, message);
            DB_Helper::bind_parameter_int(hstmt, 4, u.user_id);

            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                handle_error(SQL_HANDLE_STMT, hstmt);

            }

        }

        else
        {
            handle_error(SQL_HANDLE_STMT, hstmt);
        }
        

    }
    else
    {
        handle_error(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

}

void Audit_Log::transaction(SQLHDBC hdbc, Account& a, const string& dest_account_no, const string& status, int amount)
{
    SQLRETURN res;

    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        string query = Support::get_query("audit_log_transaction.sql");
        res = SQLPrepare(hstmt, (SQLCHAR*)query.c_str(), SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            string type = "TRANSFER";

            SQLLEN Typelen, Statuslen;

            DB_Helper::bind_parameter_string(hstmt, 1, Typelen, type);
            DB_Helper::bind_parameter_int(hstmt, 2, a.account_id);
            DB_Helper::bind_parameter_int(hstmt, 3, a.account_id); //check status dest_account
            DB_Helper::bind_parameter_int(hstmt, 4, amount);
            DB_Helper::bind_parameter_string(hstmt, 5, Statuslen, status);

            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                handle_error(SQL_HANDLE_STMT, hstmt);

            }

        }

        else
        {
            handle_error(SQL_HANDLE_STMT, hstmt);
        }
        

    }
    else
    {
        handle_error(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
    }

    // SQLMoreResults(hstmt); //dùng để chuyển sang result tiếp theo
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    
}