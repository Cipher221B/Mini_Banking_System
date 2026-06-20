#include "user_repository.h"
#include "Shared/utils/odbc_helper_error.h"
#include "Shared/utils/db_helper.h"
#include "Shared/utils/file_handle.h"
#include "Shared/error/database_errors.h"
#include "utils/repository_error_handle.h"
using namespace std;

User_Repository::User_Repository(SQLHDBC conn)
{
    hdbc = conn; 
}

Repository_Error::BUSSINESS_ERROR User_Repository::get_user_data_to_authentication(User& u, Session& s, string& phone_number)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "SELECT u.UserID, u.Full_Name, u.Status_User, u.Session_Version, ur.RoleID, us.Passwords, us.Salt FROM Users u JOIN User_Security us ON u.UserID = us.UserID JOIN User_Role ur ON u.UserID = ur.UserID WHERE u.Phone_Number = ?";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            SQLLEN phone_number_len = SQL_NTS;
            DB_Helper::bind_parameter_string(hstmt, 1, phone_number_len, phone_number);
            res = SQLExecute(hstmt);

            if(SQL_SUCCEEDED(res))
            {
                int user_id, session_version, role_id;
                char status_user[15], full_name[50];
                vector<unsigned char> credential(32), salt(16);
                
                SQLLEN status_len = SQL_NTS;
                SQLLEN full_name_len = SQL_NTS;
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
                    u.set_credential(credential);
                    u.set_salt(salt);
                }
                else if(res == SQL_NO_DATA) //Can't catch this error with native error because sql servser doesn't response err this case
                {
                    return Repository_Error::BUSSINESS_ERROR::PHONE_NUMBER_DOES_NOT_EXIST;
                }
                else
                {

                    Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__); //throw sai logic

                }
            }
            else
            {
                Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
            }
            
        }
        else
        {
            Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return  Repository_Error::BUSSINESS_ERROR::NONE;

}

void User_Repository::get_authentication_data(User& u)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        // cout << "Alloc success\n";
        SQLCHAR query[] = "SELECT u.Status_User, us.Passwords, us.Salt FROM Users u JOIN User_Security us ON u.UserID = us.UserID WHERE u.UserID = ?";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            int uid = u.get_user_id();
            DB_Helper::bind_parameter_int(hstmt, 1, uid);
            res = SQLExecute(hstmt);

            if(SQL_SUCCEEDED(res))
            {
                char status_user[15];
                vector<unsigned char> credential(32), salt(16);
                
                SQLLEN status_len = SQL_NTS;
                SQLLEN credential_len = u.get_credential().size();
                SQLLEN salt_len = u.get_salt().size();
                DB_Helper::bind_col_string(hstmt, 1, status_len, sizeof(status_user), status_user);
                res = SQLFetch(hstmt);

                if(SQL_SUCCEEDED(res))
                {
                    DB_Helper::get_data_vector(hstmt, 2, credential_len, credential);
                    DB_Helper::get_data_vector(hstmt, 3, salt_len, salt);
                    u.set_status_user(string(status_user));
                    u.set_credential(credential);
                    u.set_salt(salt);
                }
                else
                {
                    Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__); //throw sai logic

                }
            }
            else
            {
                Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
            }
            
        }
        else
        {
            Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
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
        SQLCHAR query[] = "SELECT u.Session_Version FROM Users u WHERE u.UserID = ?";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            int uid = s.get_user_id();
            DB_Helper::bind_parameter_int(hstmt, 1, uid);
            res = SQLExecute(hstmt);

            if(SQL_SUCCEEDED(res))
            {
                DB_Helper::bind_col_int(hstmt, 1, temp_version);
                res = SQLFetch(hstmt);

                if(!SQL_SUCCEEDED(res))
                {
                    Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);

                }

            }
            else
            {
                Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
            }
            
        }
        else
        {
            Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return temp_version;
}

Repository_Error::BUSSINESS_ERROR User_Repository::get_status_user_info(Status_User_Info& sui)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "SELECT u.UserID, u.Full_Name, u.Status_User FROM Users u WHERE u.Phone_Number = ?";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            SQLLEN phonelen = SQL_NTS;
            DB_Helper::bind_parameter_string(hstmt, 1, phonelen, sui.phone_number);
            res = SQLExecute(hstmt);

            if(SQL_SUCCEEDED(res))
            {

                char full_name[50];
                char status_user[15];

                SQLLEN namelen = SQL_NTS;
                SQLLEN statuslen = SQL_NTS;

                DB_Helper::bind_col_int(hstmt, 1, sui.userid);
                DB_Helper::bind_col_string(hstmt, 2, namelen, sizeof(full_name), full_name);
                DB_Helper::bind_col_string(hstmt, 3, statuslen, sizeof(status_user), status_user);
                res = SQLFetch(hstmt);

                if(res == SQL_NO_DATA)
                {
                    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
                    return  Repository_Error::BUSSINESS_ERROR::PHONE_NUMBER_DOES_NOT_EXIST;
                }
                else if(!SQL_SUCCEEDED(res))
                {
                    Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__); 

                }
                sui.full_name = string(full_name);
                sui.current_status = string(status_user);

            }
            else
            {
                Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
            }
            
        }
        else
        {
            Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return  Repository_Error::BUSSINESS_ERROR::NONE;
}

void User_Repository::change_status_user(Status_User_Info& sui)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "UPDATE Users SET Status_User = ?, Session_Version = Session_Version + 1 WHERE UserID = ?";

        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            SQLLEN new_statuslen = SQL_NTS;
            DB_Helper::bind_parameter_string(hstmt, 1, new_statuslen, sui.new_status);
            DB_Helper::bind_parameter_int(hstmt, 2, sui.userid);
            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                Repository_Handle::handle_error<Change_Status_User_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
            }
               
        }
        else
        {
            Repository_Handle::handle_error<Change_Status_User_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Repository_Handle::handle_error<Change_Status_User_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

}

void User_Repository::update_session_version(int uid)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "UPDATE Users SET Session_Version = Session_Version + 1 WHERE UserID = ?";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            DB_Helper::bind_parameter_int(hstmt, 1, uid);
            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                Repository_Handle::handle_error<Query_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
            }
               
        }
        else
        {
            Repository_Handle::handle_error<Query_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Repository_Handle::handle_error<Query_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

}

Repository_Error::BUSSINESS_ERROR User_Repository::add_new_user(User& u) // error from there is alway system error
{
    SQLRETURN res;

    SQLHSTMT hstmt = SQL_NULL_HSTMT; //variable statement
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt); 
    if(SQL_SUCCEEDED(res))
    {
        string query = File_Handle::get_query("query_sql/create_user.sql");
        res = SQLPrepareA(hstmt, (SQLCHAR* )query.c_str(), SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            int r = u.get_role_id();
            string fn = u.get_full_name();
            string pn = u.get_phone_number();
            string st = u.get_status_user();
            vector<unsigned char> c = u.get_credential();
            vector<unsigned char> sa = u.get_salt();

            SQLLEN full_name_len = SQL_NTS;
            SQLLEN phone_number_len = SQL_NTS;
            SQLLEN status_len = SQL_NTS;
            SQLLEN credential_len = c.size();
            SQLLEN salt_len = sa.size();

            DB_Helper::bind_parameter_string(hstmt, 1, full_name_len , fn);
            DB_Helper::bind_parameter_string(hstmt, 2, phone_number_len, pn);
            DB_Helper::bind_parameter_string(hstmt, 3, status_len, st);
            DB_Helper::bind_parameter_vector(hstmt, 4, credential_len, c);
            DB_Helper::bind_parameter_vector(hstmt, 5, salt_len, sa);
            DB_Helper::bind_parameter_int(hstmt, 6, r);
            //run statement has been prepare
            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                Repository_Handle::handle_error<Create_New_User_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
                return Repository_Error::BUSSINESS_ERROR::DUPLICATE_PHONE_NO;
                
            }
            //fetch data
            int temp_user_id;
            DB_Helper::bind_col_int(hstmt, 1, temp_user_id);
            res = SQLFetch(hstmt);
            if(!SQL_SUCCEEDED(res))
            {
                Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
            }
            u.set_user_id(temp_user_id);
            
        }
        else
        {
            Repository_Handle::handle_error<Create_New_User_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);

        }
    }
    else
    {
        Repository_Handle::handle_error<Create_New_User_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);

    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return Repository_Error::BUSSINESS_ERROR::NONE;

}

void User_Repository::change_credential(User& u, vector<unsigned char>& new_salt, vector<unsigned char>& new_credential)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "UPDATE User_Security SET Passwords = ?, Salt = ? WHERE UserID = ?";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            int uid = u.get_user_id();
            SQLLEN credential_len = new_credential.size();
            SQLLEN salt_len = new_salt.size();

            DB_Helper::bind_parameter_vector(hstmt, 1, credential_len, new_credential);
            DB_Helper::bind_parameter_vector(hstmt, 2, salt_len, new_salt);
            DB_Helper::bind_parameter_int(hstmt, 3, uid);
            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                Repository_Handle::handle_error<Change_Password_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
            }
               
        }
        else
        {
            Repository_Handle::handle_error<Change_Password_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Repository_Handle::handle_error<Change_Password_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
}