#pragma once

#include<windows.h>
#include <sqlext.h>
#include <vector>
#include <string>
using namespace std;

class DB_Helper
{
    public:

        //handle err get data api
        static void handle_error_get_data(SQLSMALLINT type, SQLHANDLE handle);
        static void handle_error_get_data(SQLSMALLINT type_err, SQLHANDLE handle_err, SQLSMALLINT type_free, SQLHANDLE handle_free);

        //handle err user api
        static void handle_error_user_register(SQLSMALLINT type, SQLHANDLE handle);
        static void handle_error_user_register(SQLSMALLINT type_err, SQLHANDLE handle_err, SQLSMALLINT type_free, SQLHANDLE handle_free);
        static void handle_error_user_login(SQLSMALLINT type_err, SQLHANDLE handle_err, SQLSMALLINT type_free, SQLHANDLE handle_free);
        static void handle_error_user_login(SQLSMALLINT type, SQLHANDLE handle);

        //handle err account api
        static void handle_error_account_register(SQLSMALLINT type_err, SQLHANDLE handle_err, SQLSMALLINT type_free, SQLHANDLE handle_free);
        static void handle_error_account_register(SQLSMALLINT type, SQLHANDLE handle);
        static void handle_error_account_login(SQLSMALLINT type, SQLHANDLE handle);
        static void handle_error_account_login(SQLSMALLINT type_err, SQLHANDLE handle_err, SQLSMALLINT type_free, SQLHANDLE handle_free);
        static void handle_error_account_transaction(SQLSMALLINT type, SQLHANDLE handle);
        static void handle_error_account_transaction(SQLSMALLINT type_err, SQLHANDLE handle_err, SQLSMALLINT type_free, SQLHANDLE handle_free);

        //wrapper
        static void bind_parameter_int(SQLHSTMT hstmt, int col, const int& value);
        static void bind_parameter_bool(SQLHSTMT hstmt, int col, const bool& value);
        static void bind_parameter_string(SQLHSTMT hstmt, int col, SQLLEN& value_len, const string& value);
        static void bind_parameter_vector(SQLHSTMT hstmt, int col, SQLLEN& value_len, const vector<unsigned char>& value);

        static void bind_col_int(SQLHSTMT hstmt, int col, int& value);
        static void bind_col_double(SQLHSTMT hstmt, int col, double& value);
        static void bind_col_string(SQLHSTMT hstmt, int col, SQLLEN& value_len, SQLLEN buffer_size, char* value);
        
        static void get_data_string(SQLHSTMT hstmt, int col, SQLLEN& value_len, char* value);
        static void get_data_vector(SQLHSTMT hstmt, int col, SQLLEN& value_len, vector<unsigned char>& value);

        //ACID api
        static void begin_transaction(SQLHDBC hdbc);
        static void rollback(SQLHDBC hdbc);
        static void commit(SQLHDBC hdbc);


        
};