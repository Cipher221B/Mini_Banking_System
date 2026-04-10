#include "db_helper.h"
#include "database_errors.h"
#include "sql_error.h"
#include "odbc_helper_error.h"
using namespace std;

//handle error get data 
void DB_Helper::handle_error_get_data(SQLSMALLINT type, SQLHANDLE handle)
{
    Error_Info last_err = Helper_Error::get_infor_error(type, handle);
    SQLFreeHandle(type, handle);
    throw Get_Data_Error(last_err.message_err, __FILE__, last_err.sql_state, last_err.native_err);
}

void DB_Helper::handle_error_get_data(SQLSMALLINT type_err, SQLHANDLE handle_err, SQLSMALLINT type_free, SQLHANDLE handle_free)
{
    Error_Info last_err = Helper_Error::get_infor_error(type_err, handle_err);
    SQLFreeHandle(type_free, handle_free);
    throw Get_Data_Error(last_err.message_err, __FILE__, last_err.sql_state, last_err.native_err);
}


//handle error user api
void DB_Helper::handle_error_user_register(SQLSMALLINT type, SQLHANDLE handle)
{
    Error_Info last_err = Helper_Error::get_infor_error(type, handle);
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

void DB_Helper::handle_error_user_register(SQLSMALLINT type_err, SQLHANDLE handle_err, SQLSMALLINT type_free, SQLHANDLE handle_free)
{
    Error_Info last_err = Helper_Error::get_infor_error(type_err, handle_err);
    SQLFreeHandle(type_free, handle_free);
    throw Create_New_User_Error(last_err.message_err, __FILE__, last_err.sql_state, last_err.native_err);

}

void DB_Helper::handle_error_user_login(SQLSMALLINT type, SQLHANDLE handle)
{
    Error_Info last_err = Helper_Error::get_infor_error(type, handle);
    SQLFreeHandle(type, handle);
    throw Get_User_Error(last_err.message_err, __FILE__, last_err.sql_state, last_err.native_err);

}

void DB_Helper::handle_error_user_login(SQLSMALLINT type_err, SQLHANDLE handle_err, SQLSMALLINT type_free, SQLHANDLE handle_free)
{
    Error_Info last_err = Helper_Error::get_infor_error(type_err, handle_err);
    SQLFreeHandle(type_free, handle_free);
    throw Get_User_Error(last_err.message_err, __FILE__, last_err.sql_state, last_err.native_err);

}

//handle error account api
void DB_Helper::handle_error_account_register(SQLSMALLINT type_err, SQLHANDLE handle_err, SQLSMALLINT type_free, SQLHANDLE handle_free)
{
    Error_Info last_err = Helper_Error::get_infor_error(type_err, handle_err);
    SQLFreeHandle(type_free, handle_free);
    throw Create_New_Account_Error(last_err.message_err, __FILE__, last_err.sql_state, last_err.native_err);

}

void DB_Helper::handle_error_account_register(SQLSMALLINT type, SQLHANDLE handle)
{
    Error_Info last_err = Helper_Error::get_infor_error(type, handle);
    if(last_err.native_err == 2627 || last_err.native_err == 2601) //DUPLICATE ACCOUNT
    {
        SQLFreeHandle(type, handle);
    }
    else
    {
        SQLFreeHandle(type, handle);
        throw Create_New_Account_Error(last_err.message_err, __FILE__, last_err.sql_state, last_err.native_err);
    }
}

void DB_Helper::handle_error_account_login(SQLSMALLINT type, SQLHANDLE handle)
{
    Error_Info last_err = Helper_Error::get_infor_error(type, handle);
    SQLFreeHandle(type, handle);
    throw Get_Account_Error(last_err.message_err, __FILE__, last_err.sql_state, last_err.native_err);

}

void DB_Helper::handle_error_account_login(SQLSMALLINT type_err, SQLHANDLE handle_err, SQLSMALLINT type_free, SQLHANDLE handle_free)
{
    Error_Info last_err = Helper_Error::get_infor_error(type_err, handle_err);
    SQLFreeHandle(type_free, handle_free);
    throw Get_Account_Error(last_err.message_err, __FILE__, last_err.sql_state, last_err.native_err);
}

void DB_Helper::handle_error_account_transaction(SQLSMALLINT type, SQLHANDLE handle)
{
    Error_Info last_err = Helper_Error::get_infor_error(type, handle);
    SQLFreeHandle(type, handle);
    throw Transaction_Error(last_err.message_err, __FILE__, last_err.sql_state, last_err.native_err);
}

void DB_Helper::handle_error_account_transaction(SQLSMALLINT type_err, SQLHANDLE handle_err, SQLSMALLINT type_free, SQLHANDLE handle_free)
{
    Error_Info last_err = Helper_Error::get_infor_error(type_err, handle_err);
    SQLFreeHandle(type_free, handle_free);
    throw Transaction_Error(last_err.message_err, __FILE__, last_err.sql_state, last_err.native_err);

}

//wrapper
void DB_Helper::bind_parameter_int(SQLHSTMT hstmt, int col, int& value)
{
    SQLBindParameter(hstmt, col, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &value, 0, NULL);
}

void DB_Helper::bind_parameter_bool(SQLHSTMT hstmt, int col, bool& value)
{
    SQLBindParameter(hstmt, col, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 0, 0, &value, 0, NULL);
}

void DB_Helper::bind_parameter_string(SQLHSTMT hstmt, int col, SQLLEN& value_len, const string& value)
{
    value_len = value.size();
    SQLBindParameter(hstmt, col, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, value_len, 0, (SQLPOINTER)value.c_str(), value_len, &value_len);
}

void DB_Helper::bind_parameter_vector(SQLHSTMT hstmt, int col, SQLLEN& value_len, vector<unsigned char>& value)
{
    value_len = value.size();
    SQLBindParameter(hstmt, col, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, value_len, 0, value.data(), value_len, &value_len);

}

void DB_Helper::bind_col_int(SQLHSTMT hstmt, int col, int& value)
{
    SQLBindCol(hstmt, col, SQL_C_LONG, &value, 0, NULL); //declare location get data each column
}

void DB_Helper::bind_col_double(SQLHSTMT hstmt, int col, double& value)
{
    SQLBindCol(hstmt, col, SQL_C_DOUBLE, &value, 0, NULL);
    
}

void DB_Helper::bind_col_string(SQLHSTMT hstmt, int col, SQLLEN& value_len, SQLLEN buffer_size, char* value)
{
    SQLBindCol(hstmt, col, SQL_C_CHAR, value, buffer_size, &value_len);

}

void DB_Helper::get_data_string(SQLHSTMT hstmt, int col, SQLLEN& value_len, char* value)
{
    SQLGetData(hstmt, col, SQL_C_CHAR, value, value_len, &value_len);
}

void DB_Helper::get_data_vector(SQLHSTMT hstmt, int col, SQLLEN& value_len, vector<unsigned char>& value)
{
    SQLGetData(hstmt, col, SQL_C_BINARY, value.data(), value_len, &value_len); //SQLLEN ngoài là độ dài length còn là kênh signal
}

//ACID
void DB_Helper::begin_transaction(SQLHDBC hdbc)
{
    SQLSetConnectAttr(hdbc, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, 0);
}

void DB_Helper::commit(SQLHDBC hdbc)
{
    SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_COMMIT);
}

void DB_Helper::rollback(SQLHDBC hdbc)
{
    SQLEndTran(SQL_HANDLE_DBC, hdbc, SQL_ROLLBACK);
}