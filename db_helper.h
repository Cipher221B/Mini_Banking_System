#pragma once

#include<windows.h>
#include <sqlext.h>
#include <vector>
#include <string>
using namespace std;

class DB_Helper
{
    public:

        static void bind_parameter_int(SQLHSTMT hstmt, int col, int& value);
        static void bind_parameter_bool(SQLHSTMT hstmt, int col, bool& value);
        static void bind_parameter_string(SQLHSTMT hstmt, int col, SQLLEN& value_len, const string& value);
        static void bind_parameter_vector(SQLHSTMT hstmt, int col, SQLLEN& value_len, vector<unsigned char>& value);

        static void bind_col_int(SQLHSTMT hstmt, int col, int& value);
        static void bind_col_double(SQLHSTMT hstmt, int col, double& value);
        static void bind_col_string(SQLHSTMT hstmt, int col, SQLLEN& value_len, SQLLEN buffer_size, char* value);
        
        static void get_data_string(SQLHSTMT hstmt, int col, SQLLEN& value_len, char* value);
        static void get_data_vector(SQLHSTMT hstmt, int col, SQLLEN& value_len, vector<unsigned char>& value);

        static void begin_transaction(SQLHDBC hdbc);
        static void rollback(SQLHDBC hdbc);
        static void commit(SQLHDBC hdbc);


        
};