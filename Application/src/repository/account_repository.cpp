#include "account_repository.h"
#include "Shared/utils/db_helper.h"
#include "Shared/utils/file_handle.h"
#include "error/bussiness_error.h"
#include "Shared/error/database_errors.h"
#include "utils/repository_error_handle.h"
using namespace std;

Account_Repository::Account_Repository(SQLHDBC conn)
{
    hdbc = conn;

}

double Account_Repository::get_balance(Account& a)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    double balance;

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "SELECT a.Balance FROM Accounts a WHERE a.UserID = ?";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            int uid = a.get_user_id();
            DB_Helper::bind_parameter_int(hstmt, 1, uid);
            res = SQLExecute(hstmt);
            if(SQL_SUCCEEDED(res))
            {
                DB_Helper::bind_col_double(hstmt, 1, balance);
                res = SQLFetch(hstmt);
                
                if(res != SQL_SUCCESS)
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
    return balance;
}

string Account_Repository::get_date_time(Transaction_Info& t)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    char time[20];

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "SELECT CONVERT(VARCHAR(19), t.At_The_Time, 120) FROM Transactions t WHERE t.Transaction_Code = ?";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            string transaction_code = t.transaction_code;
            SQLLEN code_len = SQL_NTS;

            DB_Helper::bind_parameter_string(hstmt, 1, code_len, transaction_code);
            res = SQLExecute(hstmt);
            if(SQL_SUCCEEDED(res))
            {
                SQLLEN time_len = SQL_NTS;
                DB_Helper::bind_col_string(hstmt, 1, time_len, sizeof(time), time);
                res = SQLFetch(hstmt);
                
                if(res != SQL_SUCCESS)
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
    return string(time);
}

string Account_Repository::get_status_account(Account& a)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    char status_account[15];

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "SELECT Status_Account FROM Accounts WHERE AccountID = ?";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            int aid = a.get_account_id();
            DB_Helper::bind_parameter_int(hstmt, 1, aid);
            res = SQLExecute(hstmt);
            if(SQL_SUCCEEDED(res))
            {
                SQLLEN statuslen = SQL_NTS;
                DB_Helper::bind_col_string(hstmt, 1, statuslen, sizeof(status_account), status_account);
                res = SQLFetch(hstmt);
                
                if(res != SQL_SUCCESS)
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
    return string(status_account);
}



int Account_Repository::get_sequence_account_no()
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    int sequence;

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "SELECT NEXT VALUE FOR Account_NO";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            res = SQLExecute(hstmt);

            if(SQL_SUCCEEDED(res))
            {
                DB_Helper::bind_col_int(hstmt, 1, sequence);
                res = SQLFetch(hstmt);
                
                if(res != SQL_SUCCESS)
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
    return sequence;
}

Repository_Error::BUSSINESS_ERROR Account_Repository::get_dest_account_information(Transaction_Info& t)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "SELECT a.AccountID, u.Full_Name From Accounts a JOIN Users u ON a.UserID = u.UserID WHERE a.Account_Number = ?";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            SQLLEN account_no_len = SQL_NTS;
            DB_Helper::bind_parameter_string(hstmt, 1, account_no_len, t.dest_account_no);
            res = SQLExecute(hstmt);

            if(SQL_SUCCEEDED(res))
            {
                char full_name[50];
                int temp_account_id;
                SQLLEN full_name_len;

                DB_Helper::bind_col_int(hstmt, 1, temp_account_id);
                DB_Helper::bind_col_string(hstmt, 2, full_name_len, sizeof(full_name), full_name);
                res = SQLFetch(hstmt);
                
                if(res == SQL_SUCCESS)
                {
                    t.dest_account_id = temp_account_id;
                    t.full_name = string(full_name);
                }
                else if(res == SQL_NO_DATA)
                {
                    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
                    return Repository_Error::BUSSINESS_ERROR::ACCOUNT_NOT_FOUND;
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
            Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return Repository_Error::BUSSINESS_ERROR::NONE;
}

void Account_Repository::get_account_information(Account& a)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "SELECT AccountID, Account_Number, Balance, Type_Account FROM Accounts WHERE UserID = ?";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            int user_id = a.get_user_id();
            DB_Helper::bind_parameter_int(hstmt, 1, user_id);
            res = SQLExecute(hstmt);

            if(SQL_SUCCEEDED(res))
            {
                int account_id;
                double balance;
                char account_no[15], type[20];

                SQLLEN account_no_len;
                SQLLEN type_len;

                DB_Helper::bind_col_int(hstmt, 1, account_id);
                DB_Helper::bind_col_string(hstmt, 2, account_no_len, sizeof(account_no), account_no);
                DB_Helper::bind_col_double(hstmt, 3, balance);
                DB_Helper::bind_col_string(hstmt, 4, type_len, sizeof(type), type);

                res = SQLFetch(hstmt);
                if(res == SQL_SUCCESS)
                {
                    a.set_account_id(account_id);
                    a.set_account_no(string(account_no));
                    a.set_balance(balance);
                    a.set_type(string(type));
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
            Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
}

Repository_Error::BUSSINESS_ERROR Account_Repository::get_status_account_info(Status_Account_Info& sai)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "SELECT a.AccountID, a.UserID, a.Status_Account FROM Accounts a WHERE a.Account_Number = ?";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            cout << "Prepare success\n";
            SQLLEN acc_no_len = SQL_NTS;
            DB_Helper::bind_parameter_string(hstmt, 1, acc_no_len, sai.account_no);
            res = SQLExecute(hstmt);

            if(SQL_SUCCEEDED(res))
            {
                char status_account[15];

                SQLLEN statuslen = SQL_NTS;

                DB_Helper::bind_col_int(hstmt, 1, sai.accountid);
                DB_Helper::bind_col_int(hstmt, 2, sai.userid);
                DB_Helper::bind_col_string(hstmt, 3, statuslen, sizeof(status_account), status_account);
                res = SQLFetch(hstmt);

                if(res == SQL_SUCCESS)
                {
                    sai.current_status = string(status_account);
                }
                else if(res == SQL_NO_DATA)
                {
                    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
                    return Repository_Error::BUSSINESS_ERROR::ACCOUNT_NOT_FOUND;
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
            Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return Repository_Error::BUSSINESS_ERROR::NONE;
}

void Account_Repository::change_status_account(Status_Account_Info& sai)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "UPDATE Accounts SET Status_Account = ? WHERE AccountID = ?";

        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            SQLLEN new_statuslen = SQL_NTS;

            DB_Helper::bind_parameter_string(hstmt, 1, new_statuslen, sai.new_status);
            DB_Helper::bind_parameter_int(hstmt, 2, sai.accountid);
            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                Repository_Handle::handle_error<Change_Status_Account_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
            }
               
        }
        else
        {
            Repository_Handle::handle_error<Change_Status_Account_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Repository_Handle::handle_error<Change_Status_Account_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
}

void Account_Repository::change_status_account(Status_User_Info& sui)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "UPDATE Accounts SET Status_Account = ? WHERE UserID = ?";
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

void Account_Repository::transaction(Account& a, Transaction_Info& t)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT; //variable statement
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {

        string query = File_Handle::get_query("query_sql/query_transaction.sql");
        res = SQLPrepareA(hstmt, (SQLCHAR*)query.c_str(), SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            int aid = a.get_account_id();

            DB_Helper::bind_parameter_int(hstmt, 1, aid);
            DB_Helper::bind_parameter_int(hstmt, 2, t.dest_account_id);
            DB_Helper::bind_parameter_double(hstmt, 3, t.amount);
            
            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                Repository_Handle::handle_error<Transaction_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
            }
        }
        else
        {
            Repository_Handle::handle_error<Transaction_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }
        
    }
    else
    {
        Repository_Handle::handle_error<Transaction_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
}


Repository_Error::BUSSINESS_ERROR Account_Repository::add_account(Account& a)
{
    SQLRETURN res;

    SQLHSTMT hstmt = SQL_NULL_HSTMT; //variable statement
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
        
    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "INSERT INTO Accounts(UserID, Account_Number, Type_Account, Status_Account) OUTPUT INSERTED.AccountID VALUES(?,?,?,?)";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            int uid = a.get_user_id();
            string acc_n = a.get_account_no();
            string t = a.get_type();
            string s = a.get_status();

            SQLLEN account_no_len = SQL_NTS;
            SQLLEN type_len = SQL_NTS;
            SQLLEN status_len = SQL_NTS;

            DB_Helper::bind_parameter_int(hstmt, 1, uid);
            DB_Helper::bind_parameter_string(hstmt, 2, account_no_len, acc_n);
            DB_Helper::bind_parameter_string(hstmt, 3, type_len, t);
            DB_Helper::bind_parameter_string(hstmt, 4, status_len, s);
            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                Repository_Handle::handle_error<Create_New_Account_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
                return Repository_Error::BUSSINESS_ERROR::DUPLICATE_ACCOUNT_NO;
            }

            int temp_account_id;
            DB_Helper::bind_col_int(hstmt, 1, temp_account_id); 
            res = SQLFetch(hstmt); 

            if(!SQL_SUCCEEDED(res))
            {
                Repository_Handle::handle_error<Get_Data_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
            }
            a.set_account_id(temp_account_id);
        }
        else
        {
            Repository_Handle::handle_error<Create_New_Account_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }
        
    }
    else
    {
        Repository_Handle::handle_error<Create_New_Account_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }
    
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return Repository_Error::BUSSINESS_ERROR::NONE;
}

