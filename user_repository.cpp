#include "user_repository.h"
#include "database_errors.h"
#include "odbc_helper_error.h"
#include "db_helper.h"
#include "helper_support.h"

User_Repository::User_Repository(SQLHDBC conn)
{
    hdbc = conn; 
}

void User_Repository::handle_error_register(SQLSMALLINT type, SQLHANDLE handle)
{
    last_err = Helper_Error::get_infor_error(type, handle);
    if(last_err.native_err == 2627 || last_err.native_err == 2601) //DUPLICATE ACCOUNT
    {
        SQLFreeHandle(type, handle);
    }
    else
    {
        SQLFreeHandle(type, handle);
        throw Create_New_User_Error(last_err.message_err, __FILE__, last_err.sql_state, last_err.native_err);
    }
}

void User_Repository::handle_error_register(SQLSMALLINT type_err, SQLHANDLE handle_err, SQLSMALLINT type_free, SQLHANDLE handle_free)
{
    last_err = Helper_Error::get_infor_error(type_err, handle_err);
    SQLFreeHandle(type_free, handle_free);
    throw Create_New_User_Error(last_err.message_err, __FILE__, last_err.sql_state, last_err.native_err);

}

// //User Role
void User_Repository::set_role(User& u)
{
    SQLRETURN res;

    SQLHSTMT hstmt = SQL_NULL_HSTMT; //variable statement
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt); 

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "INSERT INTO User_Role(RoleID, UserID) VALUES(?,?)";

        res = SQLPrepare(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            DB_Helper::bind_parameter_int(hstmt, 1, u.role_id);
            DB_Helper::bind_parameter_int(hstmt, 2, u.user_id);
            res = SQLExecute(hstmt);
            
            if(!SQL_SUCCEEDED(res))
            {
                handle_error_register(SQL_HANDLE_STMT, hstmt);
            }

        }
        else
        {
            handle_error_register(SQL_HANDLE_STMT, hstmt);
        }
    }
    else
    {
        handle_error_register(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

}

User_Repository::Repository_Result User_Repository::add_new_user(User& u) // error from there is alway system error
{
    SQLRETURN res;

    SQLHSTMT hstmt = SQL_NULL_HSTMT; //variable statement
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt); 

    if(SQL_SUCCEEDED(res))
    {
        cout << "Alloc success\n";
        string query = Support::get_query("create_user.sql");
        res = SQLPrepare(hstmt, (SQLCHAR* )query.c_str(), SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            cout << "Prepare success\n";

            SQLLEN full_name_len, phone_number_len, status_len;
            SQLLEN credential_len, salt_len;

            DB_Helper::bind_parameter_string(hstmt, 1, full_name_len , u.full_name);
            DB_Helper::bind_parameter_string(hstmt, 2, phone_number_len, u.phone_number);
            DB_Helper::bind_parameter_string(hstmt, 3, status_len, u.status_user);
            DB_Helper::bind_parameter_vector(hstmt, 4, credential_len, u.credential);
            DB_Helper::bind_parameter_vector(hstmt, 5, salt_len, u.salt);
            DB_Helper::bind_parameter_int(hstmt, 6, u.role_id);
            //run statement has been prepare
            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                handle_error_register(SQL_HANDLE_STMT, hstmt);
                return User_Repository::Repository_Result::DUPLICATE_PHONE_NO;
                
            }
            cout << "Execute success\n";
            //!!!
            DB_Helper::bind_col_int(hstmt, 1, u.user_id);
            SQLFetch(hstmt);
            cout << "UserID: " << u.user_id << endl;
        }
        else
        {
            handle_error_register(SQL_HANDLE_STMT, hstmt);

        }
    }
    else
    {
        handle_error_register(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);

    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    cout << "create_user_success\n";
    return User_Repository::Repository_Result::SUCCESS;

}

// void User_Repository::create_password(User& u) //nếu user tạo xong trên database nhưng create password fail thì nó có được gọi là user creat success?
// {

//     SQLRETURN res;

//     SQLHSTMT hstmt = SQL_NULL_HSTMT; //variable statement
//     res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt); 

//     if(SQL_SUCCEEDED(res))
//     {
//         SQLCHAR query[] = "INSERT INTO User_Security(UserID, Passwords, Salt) VALUES(?,?,?)";

//         res = SQLPrepare(hstmt, query, SQL_NTS);

//         if(SQL_SUCCEEDED(res))
//         {
            
//             res = SQLExecute(hstmt);
            
//             if(!SQL_SUCCEEDED(res))
//             {
//                 handle_error_register(SQL_HANDLE_STMT, hstmt);
//             }

//         }
//         else
//         {
//             handle_error_register(SQL_HANDLE_STMT, hstmt);
//         }
//     }
//     else
//     {
//         handle_error_register(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
//     }

//     SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

// }



void User_Repository::handle_error_login(SQLSMALLINT type, SQLHANDLE handle)
{
    last_err = Helper_Error::get_infor_error(type, handle);
    SQLFreeHandle(type, handle);
    throw Get_User_Error(last_err.message_err, __FILE__, last_err.sql_state, last_err.native_err);

}

void User_Repository::handle_error_login(SQLSMALLINT type_err, SQLHANDLE handle_err, SQLSMALLINT type_free, SQLHANDLE handle_free)
{
    last_err = Helper_Error::get_infor_error(type_err, handle_err);
    SQLFreeHandle(type_free, handle_free);
    throw Get_User_Error(last_err.message_err, __FILE__, last_err.sql_state, last_err.native_err);

}

void User_Repository::get_user_role(User& u)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        // cout << "Alloc success\n";
        SQLCHAR query[] = "SELECT ur.RoleID FROM User_Role ur WHERE ur.UserID = ?";
        res = SQLPrepare(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            // cout << "Prepare success\n";
            DB_Helper::bind_parameter_int(hstmt, 1, u.user_id);
            res = SQLExecute(hstmt);

            if(SQL_SUCCEEDED(res))
            {
                // cout << "Execute success\n";
                DB_Helper::bind_col_int(hstmt, 1, u.role_id);
                res = SQLFetch(hstmt);

                if(!SQL_SUCCEEDED(res))
                {
                    handle_error_login(SQL_HANDLE_STMT, hstmt);
                }

            }
            else
            {
                handle_error_login(SQL_HANDLE_STMT, hstmt);
            }
            
        }
        else
        {
            handle_error_login(SQL_HANDLE_STMT, hstmt);
        }

    }
    else
    {
        handle_error_login(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

}

void User_Repository::get_session_version(User& u, Session& s)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        // cout << "Alloc success\n";
        SQLCHAR query[] = "SELECT u.Session_Version FROM Users u WHERE u.UserID = ?";
        res = SQLPrepare(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            // cout << "Prepare success\n";
            SQLLEN phone_number_len;
            DB_Helper::bind_parameter_int(hstmt, 1, u.user_id);
            res = SQLExecute(hstmt);

            if(SQL_SUCCEEDED(res))
            {
                DB_Helper::bind_col_int(hstmt, 1, s.session_version);
                res = SQLFetch(hstmt);

                if(!SQL_SUCCEEDED(res))
                {
                    handle_error_login(SQL_HANDLE_STMT, hstmt); //throw sai logic

                }

            }
            else
            {
                handle_error_login(SQL_HANDLE_STMT, hstmt);
            }
            
        }
        else
        {
            handle_error_login(SQL_HANDLE_STMT, hstmt);
        }

    }
    else
    {
        handle_error_login(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
}

User_Repository::Repository_Result User_Repository::get_user_information(User& u, Session& s, string& phone_number)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        // cout << "Alloc success\n";
        SQLCHAR query[] = "SELECT u.UserID, u.Full_Name, u.Phone_Number, u.Status_User, u.Session_Version, us.Passwords, us.Salt FROM Users u JOIN User_Security us ON u.UserID = us.UserID WHERE u.Phone_Number = ?";
        res = SQLPrepare(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            // cout << "Prepare success\n";
            SQLLEN phone_number_len;
            DB_Helper::bind_parameter_string(hstmt, 1, phone_number_len, phone_number);
            res = SQLExecute(hstmt);

            if(SQL_SUCCEEDED(res))
            {
                // cout << "Execute success\n";
                char full_name[50], phone[15], status[15];

                SQLLEN full_name_len;
                SQLLEN phone_len;
                SQLLEN status_len;
                SQLLEN credential_len = u.credential.size();
                SQLLEN salt_len = u.salt.size();

                DB_Helper::bind_col_int(hstmt, 1, u.user_id);
                DB_Helper::bind_col_string(hstmt, 2, full_name_len, sizeof(full_name), full_name);
                DB_Helper::bind_col_string(hstmt, 3, phone_len, sizeof(phone), phone);
                DB_Helper::bind_col_string(hstmt, 4, status_len, sizeof(status), status);
                DB_Helper::bind_col_int(hstmt, 5, s.session_version);

                res = SQLFetch(hstmt);

                if(res == SQL_SUCCESS)
                {
                    // cout << "Fetch success\n";
                    DB_Helper::get_data_vector(hstmt, 6, credential_len, u.credential);
                    // cout << "Already done get credential vector\n";
                    DB_Helper::get_data_vector(hstmt, 7, salt_len, u.salt);

                    u.full_name = string(full_name);
                    u.phone_number = string(phone);
                    u.status_user = string(status);
                }
                else if(res == SQL_NO_DATA) //Can't catch this error with native error because sql servser doesn't response err this case
                {
                    //phone number did'n exist
                    return User_Repository::Repository_Result::PHONE_NUMBER_DOES_NOT_EXIST;
                }
                else
                {
                    handle_error_login(SQL_HANDLE_STMT, hstmt); //throw sai logic

                }

            }
            else
            {
                handle_error_login(SQL_HANDLE_STMT, hstmt);
            }
            
        }
        else
        {
            handle_error_login(SQL_HANDLE_STMT, hstmt);
        }

    }
    else
    {
        handle_error_login(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return User_Repository::Repository_Result::SUCCESS;

}