#include "log_repository.h"
#include <optional>
#include "Shared/utils/db_helper.h"
#include "Shared/error/database_errors.h"
#include "error/collector_error_handle.h"
#include "normalize/log_normalizer.h"

Log_Repository::Log_Repository(SQLHDBC conn)
{
    hdbc = conn;
}

int Log_Repository::get_check_point_id(int category_id)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    int id;

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "SELECT c.Last_ID_From_Last_Read FROM Collector_CheckPoint c WHERE c.CategoryID = ?";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            DB_Helper::bind_parameter_int(hstmt, 1, category_id);
            res = SQLExecute(hstmt);
            if(SQL_SUCCEEDED(res))
            {
                DB_Helper::bind_col_int(hstmt, 1, id);
                res = SQLFetch(hstmt);
                
                if(res != SQL_SUCCESS)
                {
                    Collector_Handle::handle_error<Data_Access_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
                }
            }
            else
            {
                Collector_Handle::handle_error<Data_Access_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
            }
            
        }
        else
        {
            Collector_Handle::handle_error<Data_Access_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Collector_Handle::handle_error<Data_Access_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return id;
}

void Log_Repository::update_checkpoint(int category_id, int new_checkpoint)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    if(SQL_SUCCEEDED(res))
    {
        // cout << "Category id: " << category_id << endl;
        // cout << "New Check point: " << new_checkpoint << endl;
        SQLCHAR query[] = "UPDATE Collector_CheckPoint SET Last_ID_From_Last_Read = ? WHERE CategoryID = ?";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            DB_Helper::bind_parameter_int(hstmt, 1, new_checkpoint);
            DB_Helper::bind_parameter_int(hstmt, 2, category_id);
            res = SQLExecute(hstmt);

            if(!SQL_SUCCEEDED(res))
            {
                Collector_Handle::handle_error<Query_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
            }
               
        }
        else
        {
            Collector_Handle::handle_error<Query_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Collector_Handle::handle_error<Query_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

}

TransactionBatch Log_Repository::load_transaction_batch_log(int check_point)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

    int last_id_load = check_point;
    vector<TransactionLog> list_transaction_log = {};
    // cout << "Check point of transaction log: " << check_point << endl;

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "SELECT * FROM Transactions WHERE TransactionID > ?";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            DB_Helper::bind_parameter_int(hstmt, 1, check_point);
            res = SQLExecute(hstmt);

            if(SQL_SUCCEEDED(res))
            {
                int temp_from_accountid, temp_to_accountid;
                double temp_amount;

                char transaction_code[25];
                char timestamp[30];
                char type[25];
                char status[10];

                SQLLEN transaction_len = SQL_NTS;
                SQLLEN timestamp_len = SQL_NTS;
                SQLLEN type_len = SQL_NTS;
                SQLLEN status_len = SQL_NTS;

                DB_Helper::bind_col_int(hstmt, 1, last_id_load);
                DB_Helper::bind_col_string(hstmt, 2, transaction_len, sizeof(transaction_code), transaction_code);
                DB_Helper::bind_col_string(hstmt, 3, timestamp_len, sizeof(timestamp), timestamp);
                DB_Helper::bind_col_string(hstmt, 4, type_len, sizeof(type), type);
                DB_Helper::bind_col_int(hstmt, 5, temp_from_accountid);
                DB_Helper::bind_col_int(hstmt, 6, temp_to_accountid);
                DB_Helper::bind_col_double(hstmt, 7, temp_amount);
                DB_Helper::bind_col_string(hstmt, 8, status_len, sizeof(status), status);

                while((res = SQLFetch(hstmt)) == SQL_SUCCESS)
                {
                    TransactionLog tl;
                    tl.transaction_code = string(transaction_code);
                    tl.timestamp = Log_Normalize::normalize_timestamp(string(timestamp));
                    tl.type = string(type);
                    tl.from_accountid = temp_from_accountid;
                    tl.to_accountid = temp_to_accountid;
                    tl.amount = temp_amount;
                    tl.status = string(status);
                    list_transaction_log.push_back(tl);
                }
    
                if(res != SQL_NO_DATA)
                {
                    Collector_Handle::handle_error<Data_Access_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
                }

            }
            else
            {
                Collector_Handle::handle_error<Data_Access_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
            }
            
        }
        else
        {
            Collector_Handle::handle_error<Data_Access_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Collector_Handle::handle_error<Data_Access_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    // cout << "Size of List transaction log: " << list_transaction_log.size() << endl;
    return {last_id_load, list_transaction_log};
}

ActivityBatch Log_Repository::load_activity_batch_log(int check_point)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    int last_id_load = check_point;
    vector<ActivityLog> list_activity_log = {};
    // cout << "Check point of activity log: " << check_point << endl;

    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "SELECT * FROM Activity_Logs WHERE ActivityID > ?";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            DB_Helper::bind_parameter_int(hstmt, 1, check_point);
            res = SQLExecute(hstmt);

            if(SQL_SUCCEEDED(res))
            {
                int temp_userid, temp_target_userid, temp_target_accountid;
                SQLLEN target_userid_ind;
                SQLLEN target_accountid_ind;

                char timestamp[30];
                char type[25];
                char status[10];
                char detail[255];

                SQLLEN timestamp_len = SQL_NTS;
                SQLLEN type_len = SQL_NTS;
                SQLLEN status_len = SQL_NTS;
                SQLLEN detail_len = SQL_NTS;

                DB_Helper::bind_col_int(hstmt, 1, last_id_load);
                DB_Helper::bind_col_string(hstmt, 2, timestamp_len, sizeof(timestamp), timestamp);
                DB_Helper::bind_col_string(hstmt, 3, type_len, sizeof(type), type);
                DB_Helper::bind_col_string(hstmt, 4, status_len, sizeof(status), status);
                DB_Helper::bind_col_string(hstmt, 5, detail_len, sizeof(detail), detail);
                DB_Helper::bind_col_int(hstmt, 6, temp_userid);
                DB_Helper::bind_col_int(hstmt, 7, target_userid_ind, temp_target_userid);
                DB_Helper::bind_col_int(hstmt, 8, target_accountid_ind , temp_target_accountid);

                while((res = SQLFetch(hstmt)) == SQL_SUCCESS)
                {
                    ActivityLog al;
                    if(target_userid_ind == SQL_NULL_DATA)
                    {
                        al.target_userid = nullopt;
                        
                    }
                    else
                    {
                        al.target_userid = temp_target_userid;
                    }

                    if(target_accountid_ind == SQL_NULL_DATA)
                    {
                        al.target_accountid = nullopt;
                    }
                    else
                    {
                        al.target_accountid = temp_target_accountid;
                    }

                    al.userid = temp_userid;
                    al.timestamp = Log_Normalize::normalize_timestamp(string(timestamp));
                    al.type = string(type);
                    al.status = string(status);
                    al.detail = string(detail);
                    list_activity_log.push_back(al);
                }
    
                if(res != SQL_NO_DATA)
                {
                    Collector_Handle::handle_error<Data_Access_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
                }

            }
            else
            {
                Collector_Handle::handle_error<Data_Access_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
            }
            
        }
        else
        {
            Collector_Handle::handle_error<Data_Access_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Collector_Handle::handle_error<Data_Access_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    // cout << "Size of List activity log: " << list_activity_log.size() << endl;
    return {last_id_load, list_activity_log};
}

AuthBatch Log_Repository::load_auth_batch_log(int check_point)
{
    SQLRETURN res;
    SQLHSTMT hstmt = SQL_NULL_HSTMT;
    res = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
    int last_id_load = check_point;
    vector<AuthLog> list_auth_log = {};
    // cout << "Check point of auth log: " << check_point << endl;
    if(SQL_SUCCEEDED(res))
    {
        SQLCHAR query[] = "SELECT * FROM Auth_Logs WHERE LoginID > ?";
        res = SQLPrepareA(hstmt, query, SQL_NTS);

        if(SQL_SUCCEEDED(res))
        {
            DB_Helper::bind_parameter_int(hstmt, 1, check_point);
            res = SQLExecute(hstmt);

            if(SQL_SUCCEEDED(res))
            {
                int temp_userid, temp_target_accountid;

                SQLLEN userid_ind;
                SQLLEN target_accountid_ind;

                char timestamp[30];
                char action[20];
                char status[10];
                char detail[25];
                char ip_address[45];

                
                SQLLEN timestamp_len = SQL_NTS;
                SQLLEN action_len = SQL_NTS;
                SQLLEN status_len = SQL_NTS;
                SQLLEN detail_len = SQL_NTS;
                SQLLEN ip_address_len = SQL_NTS;

                DB_Helper::bind_col_int(hstmt, 1, last_id_load);
                DB_Helper::bind_col_string(hstmt, 2, timestamp_len, sizeof(timestamp), timestamp);
                DB_Helper::bind_col_string(hstmt, 3, action_len, sizeof(action), action);
                DB_Helper::bind_col_string(hstmt, 4, status_len, sizeof(status), status);
                DB_Helper::bind_col_string(hstmt, 5, detail_len, sizeof(detail), detail);
                DB_Helper::bind_col_int(hstmt, 6, userid_ind, temp_userid);
                DB_Helper::bind_col_int(hstmt, 7, target_accountid_ind, temp_target_accountid);
                DB_Helper::bind_col_string(hstmt, 8, ip_address_len, sizeof(ip_address), ip_address);
                
                while((res = SQLFetch(hstmt)) == SQL_SUCCESS)
                {
                    AuthLog al;

                    if(userid_ind == SQL_NULL_DATA)
                    {
                        al.userid = nullopt;
                        
                    }
                    else
                    {
                        al.userid = temp_userid;
                    }
                    
                    if(target_accountid_ind == SQL_NULL_DATA)
                    {
                        al.target_accountid = nullopt;
                    }
                    else
                    {
                        al.target_accountid = temp_target_accountid;
                    }

                    al.timestamp = Log_Normalize::normalize_timestamp(string(timestamp));
                    al.action = string(action);
                    al.status = string(status);
                    al.detail = string(detail);
                    al.ip_address = string(ip_address);

                    list_auth_log.push_back(al);
                }
    
                if(res != SQL_NO_DATA)
                {
                    Collector_Handle::handle_error<Data_Access_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
                }

            }
            else
            {
                Collector_Handle::handle_error<Data_Access_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
            }
            
        }
        else
        {
            Collector_Handle::handle_error<Data_Access_Error>(SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
        }

    }
    else
    {
        Collector_Handle::handle_error<Data_Access_Error>(SQL_HANDLE_DBC, hdbc, SQL_HANDLE_STMT, hstmt, __FILE__, __LINE__);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    // cout << "Size of List auth log: " << list_auth_log.size() << endl;
    return {last_id_load, list_auth_log};
}