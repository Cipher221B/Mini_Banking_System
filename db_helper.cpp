#include "db_helper.h"
#include "database_errors.h"
#include "sql_error.h"
#include "odbc_helper_error.h"
using namespace std;

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