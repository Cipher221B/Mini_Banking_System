#include "account_repository.h"
#include "db_helper.h"
#include "helper_support.h"

Account_Repository::Account_Repository(SQLHDBC conn)
{
    hdbc = conn;

}

int Account_Repository::get_sequence_account_no(Account& a)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    int sequence;

    if(SQL_SUCCEEDED(res))
    {
        //nếu case status account = lock thì sao????
        SQLCHAR query[] = "SELECT NEXT VALUE FOR Account_NO";
        res = SQLPrepare(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            res = SQLExecute(hstmt);

            if(SQL_SUCCEEDED(res))
            {
                //!!!
                DB_Helper::bind_col_int(hstmt, 1, sequence);
                res = SQLFetch(hstmt);
                
                if(res != SQL_SUCCESS)
                {
                    DB_Helper::handle_error_get_data(SQL_HANDLE_STMT, hstmt);
                }

            }
            else
            {
                DB_Helper::handle_error_account_register(SQL_HANDLE_STMT, hstmt);
            }
            
        }
        else
        {
            DB_Helper::handle_error_account_register(SQL_HANDLE_STMT, hstmt);
        }

    }
    else
    {
        DB_Helper::handle_error_account_register(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return sequence % 100000000;
}


Account_Repository::Repository_Result Account_Repository::add_account(Account& a)
{
    SQLRETURN res;

    SQLHSTMT hstmt = SQL_NULL_HSTMT; //variable statement
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
        
    if(SQL_SUCCEEDED(res))
    {
        // cout << "Alloc (a) success\n";
        SQLCHAR query[] = "INSERT INTO Account(UserID, Account_NO, Type_Account, Status_Account) OUTPUT INSERTED.AccountID VALUES(?,?,?,?)";
        res = SQLPrepare(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            // cout << "Prepare (a) success\n";
            SQLLEN account_no_len, type_len, status_len;

            DB_Helper::bind_parameter_int(hstmt, 1, a.get_user_id());
            DB_Helper::bind_parameter_string(hstmt, 2, account_no_len, a.get_account_no());
            DB_Helper::bind_parameter_string(hstmt, 3, type_len, a.get_type());
            DB_Helper::bind_parameter_string(hstmt, 4, status_len, a.get_status());
        
            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                DB_Helper::handle_error_account_register(SQL_HANDLE_STMT, hstmt);
                return Repository_Result::DUPLICATE_ACCOUNT_NO;
            }
            //!!!
            // cout << "execute success\n";
            int temp_account_id;
            DB_Helper::bind_col_int(hstmt, 1, temp_account_id);  //declare location get data each column
            res = SQLFetch(hstmt); //k có account id k ghi được log, k có log = k có action

            if(!SQL_SUCCEEDED(res))
            {
                DB_Helper::handle_error_get_data(SQL_HANDLE_STMT, hstmt);
            }
            a.set_account_id(temp_account_id);
        }
        else
        {
            DB_Helper::handle_error_account_register(SQL_HANDLE_STMT, hstmt);
        }
        
    }
    else
    {
        DB_Helper::handle_error_account_register(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
    }
    
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    // cout << "fetch (a) success\n";
    return Repository_Result::SUCCESS;
}


Account_Repository::Repository_Result Account_Repository::get_information_account(Account& a)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        //nếu case status account = lock thì sao????
        SQLCHAR query[] = "SELECT AccountID, Account_NO, Balance, Type_Account, Status_Account FROM Account WHERE UserID = ?";
        res = SQLPrepare(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            DB_Helper::bind_parameter_int(hstmt, 1, a.user_id);
            //SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &a.user_id, 0, NULL);
            res = SQLExecute(hstmt);

            if(SQL_SUCCEEDED(res))
            {
                char account_no[15], type[20], status[20];

                SQLLEN account_no_len;
                SQLLEN type_len;
                SQLLEN status_len;

                DB_Helper::bind_col_int(hstmt, 1, a.account_id);
                DB_Helper::bind_col_string(hstmt, 2, account_no_len, sizeof(account_no), account_no);
                DB_Helper::bind_col_double(hstmt, 3, a.balance);
                DB_Helper::bind_col_string(hstmt, 4, type_len, sizeof(type), type);
                DB_Helper::bind_col_string(hstmt, 5, status_len, sizeof(status), status);

                res = SQLFetch(hstmt);
                
                if(res == SQL_SUCCESS)
                {

                    a.account_no = (string)account_no;
                    a.type = (string)type;
                    a.status = (string)status;

                    // cout << "Account NO: " << a.account_no << endl;
                    // cout << "Type: " << a.type << endl;
                    // cout << "Status: " << a.status << endl;

                }
                //nếu expand cho 1 user mở nhiều tài khoản (credit, current, saving,..) thì khi get về có thể account_no có vấn đề
                else if(res == SQL_NO_DATA)
                {
                    return Account_Repository::Repository_Result::ACCOUNT_NOT_FOUND;
                }
                else
                {
                    // cout << "On Error Case\n";
                    DB_Helper::handle_error_account_login(SQL_HANDLE_STMT, hstmt);
                }

            }
            else
            {
                DB_Helper::handle_error_account_login(SQL_HANDLE_STMT, hstmt);
            }
            
        }
        else
        {
            DB_Helper::handle_error_account_login(SQL_HANDLE_STMT, hstmt);
        }

    }
    else
    {
        DB_Helper::handle_error_account_login(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return Account_Repository::Repository_Result::SUCCESS;

}




// void Account_Repository::get_dest_account_information(Transaction_Info& tran_in)
// {
//     SQLRETURN res;
//     SQLHSTMT hstmt = SQL_NULL_HSTMT;
//     res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

//     if(SQL_SUCCEEDED(res))
//     {
//         //nếu case status account = lock thì sao????
//         SQLCHAR query[] = "BEGIN TRANSACTION; BEGIN TRY SELECT AccountID, UserID FROM Account WHERE Account_NO = ?";
//         res = SQLPrepare(hstmt, query, SQL_NTS);

//         if(SQL_SUCCEEDED(res))
//         {
//             DB_Helper::bind_parameter_int(hstmt, 1, a.user_id);
//             //SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &a.user_id, 0, NULL);
//             res = SQLExecute(hstmt);

//             if(SQL_SUCCEEDED(res))
//             {
//                 char account_no[15], type[20], status[20];

//                 SQLLEN account_no_len;
//                 SQLLEN type_len;
//                 SQLLEN status_len;

//                 DB_Helper::bind_col_int(hstmt, 1, a.account_id);
//                 DB_Helper::bind_col_string(hstmt, 2, account_no_len, sizeof(account_no), account_no);
//                 DB_Helper::bind_col_double(hstmt, 3, a.balance);
//                 DB_Helper::bind_col_string(hstmt, 4, type_len, sizeof(type), type);
//                 DB_Helper::bind_col_string(hstmt, 5, status_len, sizeof(status), status);

//                 res = SQLFetch(hstmt);
                
//                 if(res == SQL_SUCCESS)
//                 {

//                     a.account_no = (string)account_no;
//                     a.type = (string)type;
//                     a.status = (string)status;

//                     // cout << "Account NO: " << a.account_no << endl;
//                     // cout << "Type: " << a.type << endl;
//                     // cout << "Status: " << a.status << endl;

//                 }
//                 //nếu expand cho 1 user mở nhiều tài khoản (credit, current, saving,..) thì khi get về có thể account_no có vấn đề
//                 else if(res == SQL_NO_DATA)
//                 {
//                     return Account_Repository::Repository_Result::ACCOUNT_NOT_FOUND;
//                 }
//                 else
//                 {
//                     // cout << "On Error Case\n";
//                     handle_error_login(SQL_HANDLE_STMT, hstmt);
//                 }

//             }
//             else
//             {
//                 handle_error_login(SQL_HANDLE_STMT, hstmt);
//             }
            
//         }
//         else
//         {
//             handle_error_login(SQL_HANDLE_STMT, hstmt);
//         }

//     }
//     else
//     {
//         handle_error_login(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
//     }

//     SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
//     return Account_Repository::Repository_Result::SUCCESS;



// }

Account_Repository::Repository_Result Account_Repository::transaction(Account& a, string& dest_account, int amount)
{
    SQLRETURN res;

    SQLHSTMT hstmt = SQL_NULL_HSTMT; //variable statement
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {

        string query = Support::get_query("query_transaction.sql");
        res = SQLPrepare(hstmt, (SQLCHAR*)query.c_str(), SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            SQLLEN account_no_len, dest_account_no_len;

            DB_Helper::bind_parameter_string(hstmt, 1, account_no_len, a.account_no);
            DB_Helper::bind_parameter_string(hstmt, 2, dest_account_no_len, dest_account);
            DB_Helper::bind_parameter_int(hstmt, 3, amount);
            
            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                DB_Helper::handle_error_account_register(SQL_HANDLE_STMT, hstmt);
            }
            
            char ret[50];
            SQLLEN ret_len;

            DB_Helper::bind_col_string(hstmt, 1, ret_len, sizeof(ret), ret);
            res = SQLFetch(hstmt);

            //fetch error and system_error case
            if(!SQL_SUCCEEDED(res) || string(ret) == "SYSTEM_ERROR")
            {
                DB_Helper::handle_error_account_transaction(SQL_HANDLE_STMT, hstmt);
            }
            else
            {
                //bussiness logic error
                if(string(ret) == "SENDER_NOT_EXIST")
                {
                    return Account_Repository::Repository_Result::SENDER_NOT_EXIST;
                }
                else if(string(ret) == "RECEIVER_NOT_EXIST")
                {
                    return Account_Repository::Repository_Result::RECEIVER_NOT_EXIST;
                }
                else if(string(ret) == "SENDER_NOT_AVAILABLE")
                {
                    return Account_Repository::Repository_Result::SENDER_NOT_AVAILABLE;
                }
                else if(string(ret) == "RECEIVER_NOT_AVAILABLE")
                {
                    return Account_Repository::Repository_Result::RECEIVER_NOT_AVAILABLE;
                }
                else if(string(ret) == "INSUFFICIENT_BALANCE")
                {
                    return Account_Repository::Repository_Result::INSUFFICIENT_BALANCE;
                }

            }
            
        }
        else
        {
            DB_Helper::handle_error_account_transaction(SQL_HANDLE_STMT, hstmt);
        }
        
    }
    else
    {
        DB_Helper::handle_error_account_transaction(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return Account_Repository::Repository_Result::SUCCESS;
}

