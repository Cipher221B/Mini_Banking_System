#include "user_repository.h"
#include "database_errors.h"
#include "odbc_helper_error.h"
#include "db_helper.h"
#include "helper_support.h"

User_Repository::User_Repository(SQLHDBC conn)
{
    hdbc = conn; 
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
            DB_Helper::bind_parameter_int(hstmt, 1, u.role_);
            DB_Helper::bind_parameter_int(hstmt, 2, u.user_id);
            res = SQLExecute(hstmt);
            
            if(!SQL_SUCCEEDED(res))
            {
                DB_Helper::handle_error_user_register(SQL_HANDLE_STMT, hstmt);
            }

        }
        else
        {
            DB_Helper::handle_error_user_register(SQL_HANDLE_STMT, hstmt);
        }
    }
    else
    {
        DB_Helper::handle_error_user_register(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

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
                    DB_Helper::handle_error_user_login(SQL_HANDLE_STMT, hstmt);
                }

            }
            else
            {
                DB_Helper::handle_error_user_login(SQL_HANDLE_STMT, hstmt);
            }
            
        }
        else
        {
            DB_Helper::handle_error_user_login(SQL_HANDLE_STMT, hstmt);
        }

    }
    else
    {
        DB_Helper::handle_error_user_login(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

}

int User_Repository::get_version_db(Session& s)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    int temp_version;

    if(SQL_SUCCEEDED(res))
    {
        // cout << "Alloc success\n";
        SQLCHAR query[] = "SELECT u.Session_Version FROM Users u WHERE u.UserID = ?";
        res = SQLPrepare(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            // cout << "Prepare success\n";
            DB_Helper::bind_parameter_int(hstmt, 1, s.get_user_id());
            res = SQLExecute(hstmt);

            if(SQL_SUCCEEDED(res))
            {
                DB_Helper::bind_col_int(hstmt, 1, temp_version);
                res = SQLFetch(hstmt);

                if(!SQL_SUCCEEDED(res))
                {
                    DB_Helper::handle_error_get_data(SQL_HANDLE_STMT, hstmt); //throw sai logic

                }

            }
            else
            {
                DB_Helper::handle_error_get_data(SQL_HANDLE_STMT, hstmt);
            }
            
        }
        else
        {
            DB_Helper::handle_error_get_data(SQL_HANDLE_STMT, hstmt);
        }

    }
    else
    {
        DB_Helper::handle_error_get_data(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return temp_version;
}

User_Repository::Repository_Result User_Repository::get_user_data_to_authentication(User& u, Session& s, string& phone_number)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        // cout << "Alloc success\n";
        SQLCHAR query[] = "SELECT u.UserID, u.Full_Name, u.Status_User, u.Session_Version, ur.RoleID, us.Passwords, us.Salt FROM Users u JOIN User_Security us ON u.UserID = us.UserID JOIN User_Role ur ON u.UserID = ur.UserID WHERE u.Phone_Number = ?";
        res = SQLPrepare(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            // cout << "Prepare success\n";
            SQLLEN phone_number_len;
            DB_Helper::bind_parameter_string(hstmt, 1, phone_number_len, phone_number);
            res = SQLExecute(hstmt);

            if(SQL_SUCCEEDED(res))
            {
                int user_id, session_version, role_id;
                char status_user[15], full_name[50];
                vector<unsigned char> credential(32), salt(16);
                
                SQLLEN status_len, full_name_len;
                SQLLEN credential_len = u.get_credential().size();
                SQLLEN salt_len = u.get_salt().size();

                DB_Helper::bind_col_int(hstmt, 1, user_id);
                DB_Helper::bind_col_string(hstmt, 2, full_name_len, sizeof(full_name), full_name);
                DB_Helper::bind_col_string(hstmt, 3, status_len, sizeof(status_user), status_user);
                DB_Helper::bind_col_int(hstmt, 4, session_version);
                DB_Helper::bind_col_int(hstmt, 5, role_id);

                res = SQLFetch(hstmt);

                if(SQL_SUCCEEDED(res))
                {
                    DB_Helper::get_data_vector(hstmt, 6, credential_len, credential);
                    DB_Helper::get_data_vector(hstmt, 7, salt_len, salt);

                    //setter data
                    u.set_user_id(user_id);
                    u.set_full_name(string(full_name));
                    u.set_status_user(string(status_user));
                    s.set_session_version(session_version);
                    u.set_role_id(role_id);

                    u.set_salt(salt);
                    u.set_credential(credential);
                }
                else if(res == SQL_NO_DATA) //Can't catch this error with native error because sql servser doesn't response err this case
                {
                    //phone number did'n exist
                    return User_Repository::Repository_Result::PHONE_NUMBER_DOES_NOT_EXIST;
                }
                else
                {
                    DB_Helper::handle_error_get_data(SQL_HANDLE_STMT, hstmt); //throw sai logic

                }
            }
            else
            {
                DB_Helper::handle_error_get_data(SQL_HANDLE_STMT, hstmt);
            }
            
        }
        else
        {
            DB_Helper::handle_error_get_data(SQL_HANDLE_STMT, hstmt);
        }

    }
    else
    {
        DB_Helper::handle_error_get_data(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return User_Repository::Repository_Result::SUCCESS;

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

            SQLLEN full_name_len, phone_number_len, status_len, credential_len, salt_len;

            DB_Helper::bind_parameter_string(hstmt, 1, full_name_len , u.get_full_name());
            DB_Helper::bind_parameter_string(hstmt, 2, phone_number_len, u.get_phone_number());
            DB_Helper::bind_parameter_string(hstmt, 3, status_len, u.get_status_user());
            DB_Helper::bind_parameter_vector(hstmt, 4, credential_len, u.get_credential());
            DB_Helper::bind_parameter_vector(hstmt, 5, salt_len, u.get_salt());
            DB_Helper::bind_parameter_int(hstmt, 6, u.get_role_id());
            //run statement has been prepare
            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                DB_Helper::handle_error_user_register(SQL_HANDLE_STMT, hstmt);
                return User_Repository::Repository_Result::DUPLICATE_PHONE_NO;
                
            }
            // cout << "Execute success\n";
            
            //fetch data
            int temp_user_id;
            DB_Helper::bind_col_int(hstmt, 1, temp_user_id);
            res = SQLFetch(hstmt);
            if(!SQL_SUCCEEDED(res))
            {
                DB_Helper::handle_error_get_data(SQL_HANDLE_STMT, hstmt);
            }
            u.set_user_id(temp_user_id);
            
            // cout << "UserID: " << u.user_id << endl;
        }
        else
        {
            DB_Helper::handle_error_user_register(SQL_HANDLE_STMT, hstmt);

        }
    }
    else
    {
        DB_Helper::handle_error_user_register(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);

    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    // cout << "create_user_success\n";
    return User_Repository::Repository_Result::SUCCESS;

}